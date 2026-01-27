#include <algorithm>
#include <cmath>
#include "../../../../Utility/AsoUtility.h"
#include "../../../Player/Player.h"
#include "../../../Enemy/EnemyBase.h"
#include "EnemyChaseComponent.h"


EnemyChaseComponent::EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player, std::shared_ptr<NavGridManager> navManager)
    : EnemyComponent(owner, player), navGridManager_(navManager)
{

    // コンポーネントが初期化されたときにNavGridManagerの参照を保持
}

EnemyChaseComponent::~EnemyChaseComponent(void) {}

void EnemyChaseComponent::SetObstacle(std::vector<Transform> obstacle)
{
    if (navGridManager_)
    {
        // NavGridManagerに障害物チェックを実行させる
        navGridManager_->CheckObstacles(obstacle);
    }
}

// ==========================================================
// A*ヘルパー関数の実装
// ==========================================================

//bool EnemyChaseComponent::CompareNode::operator()(const AStarNode* a, const AStarNode* b) const
//{
//    if (a->F_Score() != b->F_Score())
//    {
//        // Fスコアが低い方を優先するため、降順ソート
//        return a->F_Score() > b->F_Score();
//    }
//    // Fスコアが同じ場合、Hスコアが低い方を優先
//    return a->H_Score_ > b->H_Score_;
//}

float EnemyChaseComponent::GetDistance(AStarNode* a, AStarNode* b)
{
    VECTOR distVec = VSub(a->worldPos_, b->worldPos_);
    return VSize(distVec);
}

float EnemyChaseComponent::GetHCost(AStarNode* a, AStarNode* b)
{
    float dx = std::abs(static_cast<float>(a->gridX_) - static_cast<float>(b->gridX_));
    float dz = std::abs(static_cast<float>(a->gridZ_) - static_cast<float>(b->gridZ_));
    return dx + dz; // マンハッタン距離
}

std::vector<AStarNode*> EnemyChaseComponent::GetNeighbors(AStarNode* node)
{
    std::vector<AStarNode*> neighbors;

    // X, Z 方向のオフセット (8方向)
    int checkX[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int checkZ[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int i = 0; i < 8; ++i){
        int neighborX = node->gridX_ + checkX[i];
        int neighborZ = node->gridZ_ + checkZ[i];

        if (navGridManager_ && neighborX >= 0 && neighborX < navGridManager_->GetGridSizeX() &&
            neighborZ >= 0 && neighborZ < navGridManager_->GetGridSizeZ()){
            AStarNode* neighborNode = navGridManager_->GetNode(neighborX, neighborZ);
            if (neighborNode)
            {
                neighbors.push_back(neighborNode);
            }
        }
    }
    return neighbors;
}

std::vector<VECTOR> EnemyChaseComponent::RetracePath(AStarNode* start, AStarNode* end)
{
    std::vector<VECTOR> path;
    AStarNode* current = end;

    while (current != start && current != nullptr){
        path.push_back(current->worldPos_);
        current = current->parent_;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void EnemyChaseComponent::SetTime(float time)
{
    time_ = time;
}

float EnemyChaseComponent::GetTime(void)
{
    return time_;
}


// ==========================================================
// A*メイン処理
// ==========================================================

std::vector<VECTOR> EnemyChaseComponent::FindPath(VECTOR startPos, VECTOR endPos)
{
    //if (!navGridManager_) return {};

    //// 探索前のノードリセット
    //navGridManager_->ResetAllNodes();

    //AStarNode* startNode = navGridManager_->NodeFromWorldPoint(startPos);
    //AStarNode* targetNode = navGridManager_->NodeFromWorldPoint(endPos);

    //// 始点または終点が無効な場合
    //if (!startNode || !targetNode || !targetNode->isWalkable_) {
    //    return {};
    //}

    //// A*リストの初期化
    //std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNode> openList;
    //std::unordered_set<AStarNode*> closedList; // 既に処理したノードを格納

    //startNode->G_Score_ = 0;
    //startNode->H_Score_ = GetHCost(startNode, targetNode);
    //openList.push(startNode);

    //while (!openList.empty())
    //{
    //    AStarNode* currentNode = openList.top();
    //    openList.pop();

    //    // ノードが既に処理済みであればスキップ
    //    if (closedList.count(currentNode)) continue;

    //    closedList.insert(currentNode);

    //    // ゴールに到達
    //    if (currentNode == targetNode)
    //    {
    //        return RetracePath(startNode, targetNode);
    //    }

    //    for (AStarNode* neighbor : GetNeighbors(currentNode))
    //    {
    //        if (!neighbor->isWalkable_ || closedList.count(neighbor))
    //        {
    //            continue;
    //        }

    //        float newGCost = currentNode->G_Score_ + GetDistance(currentNode, neighbor);

    //        if (newGCost < neighbor->G_Score_)
    //        {
    //            neighbor->G_Score_ = newGCost;
    //            neighbor->H_Score_ = GetHCost(neighbor, targetNode);
    //            neighbor->parent_ = currentNode;

    //            // Fスコアが更新されたので、優先度キューに再投入
    //            openList.push(neighbor);
    //        }
    //    }
    //}

    return {}; // 経路が見つからなかった
}


// ==========================================================
// 追跡メイン処理
// ==========================================================

void EnemyChaseComponent::Chase(float deltaTime, Transform& transform,
    VECTOR& moveDir, VECTOR& movePow, float moveSpeed, Quaternion& outRotation)
{
    // 1. A*の再計算
    pathRecalcTimer_ -= deltaTime;
    SetTime(pathRecalcTimer_);
    if (pathRecalcTimer_ <= 0.0f){
        currentPath_ = FindPath(transform.pos, player_.GetTransform().pos);
        currentPathIndex_ = 0;
        pathRecalcTimer_ = 0.3f; // 0.5秒ごとに再計算
    }

    // 2. 経路が存在しない場合
    if (currentPath_.empty() || currentPathIndex_ >= currentPath_.size()){
        moveDir = AsoUtility::VECTOR_ZERO;
        return;
    }

    // 3. 移動目標の設定と到達判定
    VECTOR targetPos = currentPath_[currentPathIndex_];

    VECTOR currentPos = transform.pos;
    currentPos.y = 0.0f;
    targetPos.y = 0.0f;

    VECTOR moveVector = VSub(targetPos, currentPos);
    float distance = VSize(moveVector);

    // ノードに到達したら次のノードへ
    if (distance < 20.0f){
        currentPathIndex_++;
        if (currentPathIndex_ >= currentPath_.size()){
            moveDir = AsoUtility::VECTOR_ZERO;
            return;
        }
        // 次のノードを目標に再設定
        targetPos = currentPath_[currentPathIndex_];
    }

    // 4. 移動方向と回転の設定
    VECTOR moveDirection = VNorm(VSub(targetPos, currentPos));
    moveDir = moveDirection;

    movePow = VScale(moveDir, moveSpeed);

    // 回転処理
    float rotationY = atan2f(moveDirection.x, moveDirection.z);
    Quaternion targetRotation = Quaternion::Euler({ 0.0f, rotationY, 0.0f });

    // 滑らかに補間
    outRotation = Quaternion::Slerp(transform.quaRot, targetRotation, 5.0f * deltaTime);
}