#include "../../../../Utility/AsoUtility.h"
#include "../../../Player/Player.h"
#include "EnemyChaseComponent.h"
#include "../../../Enemy/AStar/AStarNode.h"
#include "../../../Enemy/EnemyBase.h"

EnemyChaseComponent::EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player)
    :
    EnemyComponent(owner, player),
    currentPathIndex_(0),
    pathRecalcTimer_(0.0f),
    currentPath_{}
{
}

EnemyChaseComponent::~EnemyChaseComponent(void)
{
}

void EnemyChaseComponent::Update(float deltaTime)
{

    CapsuleCast(deltaTime);

}

void EnemyChaseComponent::RecalcTime(float deltaTime)
{
    // A*の再計算タイミング
    pathRecalcTimer_ -= deltaTime;
    if (pathRecalcTimer_ <= 0.0f)
    {
        // FindPathはもともとここにあった
        currentPathIndex_ = 0;
    }
}

void EnemyChaseComponent::Chase(float deltaTime,
    Transform& transform,
    VECTOR& moveDir,
    VECTOR& movePow,
    float moveSpeed,
    Quaternion& outRotation)
{
   
    // 1. A*の再計算タイミング
    pathRecalcTimer_ -= deltaTime;
    if (pathRecalcTimer_ <= 0.0f)
    {
        // プレイヤーの位置をゴールとして新しい経路を計算
        currentPath_ = FindPath(transform.pos, player_.GetTransform().pos);
        currentPathIndex_ = 0;
        pathRecalcTimer_ = 0.5f; // 0.5秒ごとに再計算
    }

    // 2. 経路が存在しない場合
    if (currentPath_.empty() || currentPathIndex_ >= currentPath_.size())
    {
        moveDir = AsoUtility::VECTOR_ZERO;
        return;
    }

    // 3. 移動目標の設定と到達判定
    VECTOR targetPos = currentPath_[currentPathIndex_];

    // Y軸を無視した移動計算
    VECTOR currentPos = transform.pos;
    currentPos.y = 0.0f;
    targetPos.y = 0.0f;

    VECTOR moveVector = VSub(targetPos, currentPos);
    float distance = VSize(moveVector);

    // ノードに到達したら次のノードへ
    if (distance < 5.0f)
    {
        currentPathIndex_++;
        // 最後のノードに到達したら停止
        if (currentPathIndex_ >= currentPath_.size())
        {
            moveDir = AsoUtility::VECTOR_ZERO;
            return;
        }
        // 次のノードを目標に再設定
        targetPos = currentPath_[currentPathIndex_];
    }

    // 4. 移動方向と回転の設定
    VECTOR moveDirection = VNorm(VSub(targetPos, currentPos));
    moveDir = moveDirection;

    // 回転処理
    float rotationY = atan2f(moveDirection.x, moveDirection.z);
    Quaternion targetRotation = Quaternion::Euler({ 0.0f, rotationY, 0.0f });

    // 滑らかに補間
    outRotation = Quaternion::Slerp(transform.quaRot, targetRotation, 5.0f * deltaTime);
}

void EnemyChaseComponent::SetObstacle(std::vector<Transform> obstacle)
{
    obstacle_ = obstacle;
}

void EnemyChaseComponent::SetCurrentPath(std::vector<VECTOR> currentPath)
{
    currentPath_ = currentPath;
}

float EnemyChaseComponent::GetRecalcTime(void)
{
    return pathRecalcTimer_;
}

void EnemyChaseComponent::SetRecalcTime(float time)
{
    pathRecalcTimer_ = time;
}

void EnemyChaseComponent::CapsuleCast(float deltaTime)
{

}

VECTOR EnemyChaseComponent::CalculateAvoidanceForce(const Transform& transform, float lookAheadDistance, float deltaTime)
{
    // 現在の進行方向を取得（クォータニオンから前方ベクトルを計算）
    //VECTOR forward = transform.quaRot.ToForwardVector();
    VECTOR forward = transform.quaRot.GetForward();
    forward.y = 0.0f; // 水平方向のみ考慮
    forward = VNorm(forward);

    // 敵の予測される将来の位置 (レイの終点)
    VECTOR ahead = VAdd(transform.pos, VScale(forward, lookAheadDistance));

    // 敵の正面方向の障害物をチェックするためのレイキャスト
    VECTOR avoidanceForce = AsoUtility::VECTOR_ZERO;
    float maxAvoidanceDist = 0.0f;

    // シンプルな円形/球形の障害物回避の例:
    for (const auto& obstacle : obstacle_)
    {
        //if (!obstacle) continue;

        VECTOR obsPos = obstacle.pos;
        obsPos.y = transform.pos.y; // Y軸を合わせる

        VECTOR diff = VSub(ahead, obsPos);
        float distance = VSize(diff);

        // 障害物の半径 + 敵の回避余裕 (ここでは仮に 1.5f を障害物のサイズとして利用)
        const float collisionRadius = 1.5f + 1.0f; // 例: 障害物のサイズ + 敵の回避半径

        if (distance < collisionRadius)
        {
            // 衝突が近づいている: 障害物から離れる方向の力を計算
            VECTOR awayFromObstacle = VSub(transform.pos, obsPos);
            awayFromObstacle.y = 0.0f;

            // 距離が近いほど力が強くなるように正規化し、反比例させる
            // 距離が0に近づくと力が無限大になるのを避けるため、一定値でクリップ
            // 修正後のコード:
            float strength = (collisionRadius - distance) / collisionRadius;
            if (strength < 0.0f) {
                strength = 0.0f;
            }

            VECTOR desiredVelocity = VScale(VNorm(awayFromObstacle), strength * 10.0f); // 10.0fは回避の強さ

            // 複数の障害物がある場合は、最も回避の必要な力を合成
            avoidanceForce = VAdd(avoidanceForce, desiredVelocity);

            // レイキャストを使用する場合は、衝突点と法線ベクトルから正確な回避方向を計算します。
        }
    }

    return avoidanceForce; // 回避するための「力」ベクトルを返す
}

std::vector<VECTOR> EnemyChaseComponent::FindPath(VECTOR startPos, VECTOR endPos)
{
    // 1. ノードの取得と初期リセット
    // 【重要】探索前に全ノードのG_Score, parent をリセットする必要があります
    if (navGridManager_ == nullptr) return {};
    navGridManager_->ResetAllNodes(); // 全ノードを初期状態にリセット

    AStarNode* startNode = navGridManager_->NodeFromWorldPoint(startPos);
    AStarNode* targetNode = navGridManager_->NodeFromWorldPoint(endPos);

    if (!startNode || !targetNode || !targetNode->isWalkable) {
        return {};
    }

    // 2. リストの初期化
    // openListにはノードへのポインタを格納
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNode> openList;
    // closedListには探索済みのノードを格納
    std::unordered_set<AStarNode*> closedList;

    startNode->G_Score_ = 0;
    startNode->H_Score_ = GetHCost(startNode, targetNode);
    startNode->parent_ = nullptr;
    openList.push(startNode);

    // 3. 探索メインループ
    while (!openList.empty())
    {
        AStarNode* currentNode = openList.top();
        openList.pop();

        // Fスコアが古いノードを無視（優先度キューを使用する際のテクニック）
        if (closedList.count(currentNode)) continue;

        closedList.insert(currentNode);

        // ゴール判定
        if (currentNode == targetNode)
        {
            return RetracePath(startNode, targetNode);
        }

        // 4. 隣接ノードの調査
        for (AStarNode* neighbor : GetNeighbors(currentNode))
        {
            if (!neighbor->isWalkAble_ || closedList.count(neighbor))
            {
                continue;
            }

            // 新しい経路のGコストを計算
            float newGCost = currentNode->G_Score_ + GetDistance(currentNode, neighbor);

            // 新しい経路が既存の経路より短い場合
            if (newGCost < neighbor->G_Score_)
            {
                neighbor->G_Score_ = newGCost;
                neighbor->H_Score_ = GetHCost(neighbor, targetNode);
                neighbor->parent_ = currentNode;

                // オープンリストに追加/再投入 (Fスコア順に並べ替えられる)
                openList.push(neighbor);
            }
        }
    }

    // 経路が見つからなかった
    return {};
}

float EnemyChaseComponent::GetHCost(AStarNode* a, AStarNode* b)
{
    // グリッド座標のマンハッタン距離を使う
    float dx = std::abs(a->gridX_ - b->gridX_);
    float dz = std::abs(a->gridZ_ - b->gridZ_);
    // VSizeの代わりに、GetDistanceと同じものを使用しても良い
    return dx + dz;
}

std::vector<AStarNode*> EnemyChaseComponent::GetNeighbors(AStarNode* node)
{
    std::vector<AStarNode*> neighbors;

    // X, Z 方向のオフセット
    int checkX[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int checkZ[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int i = 0; i < 8; ++i)
    {
        int neighborX = node->gridX_ + checkX[i];
        int neighborZ = node->gridZ_ + checkZ[i];

        // グリッドの有効範囲内かチェック (navGridManager_ を利用)
        if (neighborX >= 0 && neighborX < navGridManager_->GetGridSizeX() &&
            neighborZ >= 0 && neighborZ < navGridManager_->GetGridSizeZ())
        {
            // 有効なノードを取得
            AStarNode* neighborNode = navGridManager_->GetNode(neighborX, neighborZ);
            if (neighborNode)
            {
                neighbors.push_back(neighborNode);
            }
        }
    }
    return neighbors;
}

float EnemyChaseComponent::GetDistance(AStarNode* a, AStarNode* b)
{
    // グリッド移動のコストを返す (斜め移動は1.4倍など)
    // ここではユークリッド距離（ワールド座標）を使うのが最も正確です。
    // VSub, VSize は DxLib の関数と仮定
    VECTOR distVec = VSub(a->worldPos_, b->worldPos_);
    return VSize(distVec);
}

std::vector<VECTOR> EnemyChaseComponent::RetracePath(AStarNode* start, AStarNode* end)
{
    std::vector<VECTOR> path;
    AStarNode* current = end;

    while (current != start && current != nullptr)
    {
        path.push_back(current->worldPos_);
        current = current->parent_;
    }
    std::reverse(path.begin(), path.end()); // 逆順なので反転
    return path;
}

void EnemyChaseComponent::ResetAStarNodes()
{
}
