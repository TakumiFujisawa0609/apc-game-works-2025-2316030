#include <unordered_set>
#include <queue>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Collider.h"
#include "../Common/Capsule.h"
#include "../Common/AnimationController.h"
#include "../Components/Charactor/Enemy/EnemyPatrolComponent.h"
#include "../Components/Charactor/Enemy/EnemyChaseComponent.h"
#include "Patrol/PatrolNode.h"
#include "Patrol/PatrolPath.h"
#include "AStar/NavGridManager.h"
#include "../Player/Player.h"
#include "EnemyBase.h"

const std::map<EnemyBase::STATE, const wchar_t*>stateNames =
{
    {EnemyBase::STATE::PATROL,L"PATROL"},
    {EnemyBase::STATE::CHASE,L"CHASE"},
    {EnemyBase::STATE::ATTACK,L"ATTACK"},
    {EnemyBase::STATE::IDLE,L"IDLE"}
};

EnemyBase::EnemyBase(Player& player)
    :
    player_(player),
    patrolComponent_(nullptr),
    chaseComponent_(nullptr),
    isHearingSound_(false),
    isPlayerInSight_(false),
    frame_(-1),
    currentPatrolPathIndex_(-1),
    state_(STATE::IDLE),
    isWaiting_(false),
    currentWaitTime_(0.0f),
    dis_(0.0f),
    currentNode_(0),
    isDamageCheckActive_(false),
    hasPlayerBeenHit_(false),
    isAttack_(false)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(void)
{
    // モデル情報
    transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY));
    InitModel(POS, SCALE, QUAROT_LOCAL);
    InitializeCapsule(TOP, DOWN, RADIUS);
    transform_.MakeCollider(Collider::TYPE::ENEMY);

    // 状態の初期化

    moveSpeed_ = 8.0f; // 移動速度
    moveDir_ = AsoUtility::VECTOR_ZERO; // 移動方向
    movedPos_ = transform_.pos;

    state_ = STATE::IDLE;

    currentPatrolPathIndex_ = 0;

    // アニメーション
    animationController_ = std::make_unique<AnimationController>(transform_.modelId);

    // 追加予定のアニメーションの初期化
    InitAnimation();

    ChangeState(STATE::PATROL);

    // モデルの更新
    transform_.Update();
}

void EnemyBase::InitComponents(void)
{
    // 移動コンポーネント
    patrolComponent_ = AddCharactorComponent<EnemyPatrolComponent>(player_);

    // 追跡コンポ―ネント
    chaseComponent_ = AddCharactorComponent<EnemyChaseComponent>(player_, navGridManager_);

}

void EnemyBase::Update(float deltaTime)
{
    OnUpdate(deltaTime);

    animationController_->Update();

    // モデルの更新
    transform_.Update();
}

void EnemyBase::OnUpdate(float deltaTime)
{
    // 移動方向のリセット
    moveDir_ = AsoUtility::VECTOR_ZERO;

    // 移動量(movePow_)を初期化
    movePow_ = AsoUtility::VECTOR_ZERO;

    bool isChase_ = false;

    if (state_!=STATE::ATTACK&&(EyeSerch() || isHearingSound_)){
        // 追跡状態への遷移条件
        if (state_ != STATE::CHASE){
            ChangeState(STATE::CHASE);
        }
        isChase_ = true;
        frame_ = 0;
    }
    else{
        isChase_ = false;
    }

    if (!isChase_){
        frame_++;
        if (frame_ >= 180){
            if (state_ == STATE::CHASE){
                // CHASEからPATROLへ遷移する場合
                ChangeState(STATE::PATROL);

                // 最も近い巡回ノードを見つけて、それを次の目標とする
                currentPatrolPathIndex_ = patrolPath_->FindClosestNodeIndex(transform_.pos);
                currentNode_ = currentPatrolPathIndex_;
            }
        }
    }

    if ((state_ == STATE::CHASE) && isAttackRange() && !isAttack_){
        ChangeState(STATE::ATTACK);
        isAttack_ = true;
    }

    // 現在の状態に応じたコンポーネントのロジック実行
    switch (state_)
    {
    case EnemyBase::STATE::PATROL:
        UpdatePatrol(deltaTime);
        break;
    case EnemyBase::STATE::CHASE:
        UpdateChase(deltaTime);
        break;
    case EnemyBase::STATE::ATTACK:
        if (!animationController_->IsEnd()){
            HandleAttackCollsion(deltaTime);
        }
        state_;

        if (animationController_->IsEnd()){
            // ダメージ判定を無効化し、追跡状態に戻す
            isDamageCheckActive_ = false;
            hasPlayerBeenHit_ = false;
            isAttack_ = false;
            ChangeState(STATE::CHASE);
        }
        break;
    case EnemyBase::STATE::IDLE:
        break;
    default:
        break;
    }


    //// 移動
    //VECTOR horizontalMovement = VScale(moveDir_, moveSpeed_ * deltaTime);
    //movePow_ = VAdd(movePow_, horizontalMovement);

    //CalcGravityPow();

    Collision();
}

void EnemyBase::Draw(void)
{
    MV1DrawModel(transform_.modelId);

#ifdef _DEBUG

    //const wchar_t* stateName = L"UNKONOWN";
    //auto it = stateNames.find(state_);
    //if (it != stateNames.end())
    //{
    //    stateName = it->second;
    //}

    //DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 48, GetColor(255, 255, 255), L"state: %s", stateName);

    //if (capsule_)capsule_->Draw();

    //DrawDebug();

    //DrawFormatString(Application::GetInstance().GetWindowSize().width - 200, 64, GetColor(255, 255, 255), L"pos: (%.2f,%.2f,%.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
    //DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 80, GetColor(255, 255, 255), L"dis: %.2f", patrolComponent_->GetDis());
    //DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 96, GetColor(255, 255, 255), L"nextNode: %d", patrolComponent_->GetCurrentNode());
    //DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 112, GetColor(255, 255, 255), L"recalcTime: %.2f", chaseComponent_->GetTime());

#endif // _DEBUG


}

void EnemyBase::SetObstacle(std::vector<Transform> obstacle)
{
    chaseComponent_->SetObstacle(obstacle);
}

void EnemyBase::SetPatrolPath(std::shared_ptr<PatrolPath> path)
{
    patrolPath_ = path;
}

void EnemyBase::SetNavGridManagedr(std::shared_ptr<NavGridManager> navGridManager)
{
    navGridManager_ = navGridManager;
}

void EnemyBase::InitModel(VECTOR pos, VECTOR scl, VECTOR quaRotLocal)
{
    // モデルの設定
    transform_.pos = { pos.x,pos.y,pos.z };
    transform_.scl = { scl.x,scl.y,scl.z };
    transform_.quaRot = Quaternion();
    transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(quaRotLocal.x),
        AsoUtility::Deg2RadF(quaRotLocal.y),AsoUtility::Deg2RadF(quaRotLocal.z) });

    // モデル情報の更新
    transform_.Update();
}

void EnemyBase::InitAnimation(void)
{
    std::wstring path = Application::PATH_MODEL + L"Enemy/Y-bot/";

    animationController_->Add((int)ANIM::PATROL, path + L"Walk With Briefcase.mv1", 20.0f);
    animationController_->Add((int)ANIM::CHASE, path + L"Jog Forward.mv1", 25.0f);
    animationController_->Add((int)ANIM::ATTACK, path + L"Zombie Attack.mv1", 25.0f);
    animationController_->Add((int)ANIM::IDLE, path + L"Mutant Idle.mv1", 30.0f);
    animationController_->Add((int)ANIM::LOOK, path + L"Look Around.mv1", 25.0f);
}

void EnemyBase::Rotate(void)
{
    Charactor::Rotate();
}

void EnemyBase::Collision(void)
{
    // 現在座標を起点に移動後座標を決める
    movedPos_ = VAdd(transform_.pos, movePow_);

    //Charactor::Collision();

    // 移動
    transform_.pos = movedPos_;
    transform_.pos.y = 13.0f;
}

void EnemyBase::CollisionCapsule(void)
{
    /*Charactor::CollisionCapsule();*/

    // カプセルを移動させる
    Transform trans = Transform(transform_);
    trans.pos = movedPos_;
    trans.Update();
    Capsule cap = Capsule(*capsule_, &trans);

    // カプセルとの衝突判定
    for (const auto& c : colliders_){
        auto hits = MV1CollCheck_Capsule(
            c->modelId_, -1,
            cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());
        for (int i = 0; i < hits.HitNum; i++){
            auto hit = hits.Dim[i];
            for (int tryCnt = 0; tryCnt < 10; tryCnt++){
                int pHit = HitCheck_Capsule_Triangle(
                    cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
                    hit.Position[0], hit.Position[1], hit.Position[2]);
                if (pHit){
                    movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));

                    // カプセルを移動させる
                    trans.pos = movedPos_;
                    trans.Update();
                    continue;
                }
                break;
            }
        }
        MV1CollResultPolyDimTerminate(hits);
    }
}

void EnemyBase::CollisionGravity(void)
{
    Charactor::CollisionGravity();
}

void EnemyBase::CalcGravityPow(void)
{
    Charactor::CalcGravityPow();
}

void EnemyBase::ChangeState(STATE state)
{
    state_ = state;

    isDamageCheckActive_ = false;
    hasPlayerBeenHit_ = false;

    switch (state_)
    {
    case EnemyBase::STATE::PATROL:
        animationController_->Play((int)ANIM::PATROL, true, 1.0f);
        break;
    case EnemyBase::STATE::CHASE:
        animationController_->Play((int)ANIM::CHASE, true, 0.5f);
        break;
    case EnemyBase::STATE::ATTACK:
        animationController_->Play((int)ANIM::ATTACK, false, 0.5f);
        isDamageCheckActive_ = true;
        break;
    case EnemyBase::STATE::IDLE:
        animationController_->Play((int)ANIM::IDLE, true, 0.5f);
        break;
    default:
        break;
    }

}

bool EnemyBase::EyeSerch(void)
{
    // プレイヤーの座標を取得
    VECTOR pPos = player_.GetTransform().pos;

    // エネミーからプレイヤーまでのベクトル
    VECTOR diff = VSub(pPos, transform_.pos);

    // 視野範囲に入っているか判断
    float distance = std::pow(diff.x, 2.0f) + std::pow(diff.z, 2.0f);
    if (distance <= (std::pow(EYE_VIEW_RANGE, 2.0f))){
        // 自分から見たプレイヤーの角度を求める
        float rad = atan2(pPos.x - transform_.pos.x, pPos.z - transform_.pos.z);
        float viewRad = rad - transform_.rot.y;
        float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));

        // 視野角内に入っているか判断
        if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE)){
            auto info = MV1CollCheck_Line(player_.GetTransform().modelId, -1,
                { transform_.pos.x,transform_.pos.y,transform_.pos.z },
                { player_.GetTransform().pos.x,player_.GetTransform().pos.y,player_.GetTransform().pos.z });
            if (info.HitFlag == 0){
                return true;
            }
        }
    }

    return false;
}

bool EnemyBase::HearingSound(void)
{
    // 音の大きさをプレイヤーまた、オブジェクト取得
    // 対象の音ハンドル
    int soundH_ = -1;

    // 音の大きさ
    int soundVol_ = -1;

    return false;
}

void EnemyBase::DrawDebug(void)
{

    // ラジアンに変換
    float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

    // 角度から方向を取得
    VECTOR rad = transform_.quaRot.ToEuler();

    // 前方方向
    float x1 = sinf(rad.y);
    float z1 = cosf(rad.y);

    // 右斜め30度の方向
    float x2 = sinf(rad.y + viewRad);
    float z2 = cosf(rad.y + viewRad);

    // 左斜め30度の方向
    float x3 = sinf(rad.y - viewRad);
    float z3 = cosf(rad.y - viewRad);

    // 自分の座標
    VECTOR pos0 = transform_.pos;

#pragma region 視野範囲の描画

    // 前方方向
    VECTOR Ipos1 = transform_.pos;
    Ipos1.x += x1 * EYE_VIEW_RANGE;
    Ipos1.z += z1 * EYE_VIEW_RANGE;

    // 右斜め30度の方向
    VECTOR Ipos2 = transform_.pos;
    Ipos2.x += x2 * EYE_VIEW_RANGE;
    Ipos2.z += z2 * EYE_VIEW_RANGE;

    // 左斜め30度の方向
    VECTOR Ipos3 = transform_.pos;
    Ipos3.x += x3 * EYE_VIEW_RANGE;
    Ipos3.z += z3 * EYE_VIEW_RANGE;

    DrawTriangle3D(pos0, Ipos1, Ipos2, 0xffdead, true);
    DrawTriangle3D(pos0, Ipos3, Ipos1, 0xffdead, true);

#pragma endregion

#pragma region 攻撃範囲の描画

    // 前方向
    VECTOR Apos1 = transform_.pos;
    Apos1.x += x1 * ATTACK_RANGE;
    Apos1.z += z1 * ATTACK_RANGE;

    // 右斜め30度の方向
    VECTOR Apos2 = transform_.pos;
    Apos2.x += x2 * ATTACK_RANGE;
    Apos2.z += z2 * ATTACK_RANGE;

    // 左斜め30度の方向
    VECTOR Apos3 = transform_.pos;
    Apos3.x += x3 * ATTACK_RANGE;
    Apos3.z += z3 * ATTACK_RANGE;

    //DrawTriangle3D(pos0, Apos1, Apos2, 0xff4500, true);
    //DrawTriangle3D(pos0, Apos3, Apos1, 0xff4500, true);


#pragma endregion
}

void EnemyBase::UpdatePatrol(float deltaTime)
{
    // outMoveDirを初期化 (移動しない場合のため)
    moveDir_ = AsoUtility::VECTOR_ZERO;

    // 現在の目標ノードを取得
    const PatrolNode& targetNode = patrolPath_->GetNodeIndex(currentPatrolPathIndex_);
    VECTOR targetPos = targetNode.GetPos();
    currentNode_ = currentNode_;

    // ----------------------------------------------------
    // 待機処理
    // ----------------------------------------------------
    if (isWaiting_){
        currentWaitTime_ -= deltaTime;
        if (currentWaitTime_ <= 0.0f){
            // 待機終了。次のノードへ進む
            isWaiting_ = false;

            // 状態をPATROL（移動）に戻す
            //enemyBase->ChangeState(EnemyBase::STATE::PATROL);
            // 次のノードのインデックスを更新
            patrolPath_->GetNextNode(currentPatrolPathIndex_);
            currentNode_ = currentPatrolPathIndex_;

            return;
        }
        else{
            // 待機中は移動を停止
            moveDir_ = AsoUtility::VECTOR_ZERO;

            // 回転はそのまま維持
            transform_.quaRot = transform_.quaRot;
            return;
        }
    }

    // ----------------------------------------------------
    // 移動/回転処理
    // ----------------------------------------------------

    // 現在位置（Y軸は無視して水平移動）
    VECTOR currentPos = transform_.pos;
    currentPos.y = 0.0f;
    targetPos.y = 0.0f;

    VECTOR moveVector = VSub(targetPos, currentPos);
    float distance = VSize(moveVector);
    dis_ = distance;

    // 目標位置に到達したかをチェック（許容誤差1.0f）
    if (distance < 5.5f){
        // 目標に到達したら待機状態に遷移
        isWaiting_ = true;
        currentWaitTime_ = targetNode.GetWaitTime();

        // 移動を停止
        moveDir_ = AsoUtility::VECTOR_ZERO;
    }
    else{
        // 移動方向を設定 (出力)
        VECTOR moveDirection = VNorm(moveVector);
        moveDir_ = moveDirection;
        movePow_ = VScale(moveDir_, 3.0f);

        float length = AsoUtility::MagnitudeF(moveDir_);
        if (length < 0.001f)return;

        // モデルの回転角度を設定 (出力)
        // Z軸（前方向）から移動方向への回転
        float rotationY = atan2f(moveDirection.x, moveDirection.z);
        Quaternion targetRotation = Quaternion::Euler({ 0.0f, rotationY, 0.0f });

        // 現在の回転と目標の回転の間を滑らかに補間
        transform_.quaRot = Quaternion::Slerp(transform_.quaRot, targetRotation, 5.0f * deltaTime);
    }
}

void EnemyBase::UpdateChase(float deltaTime)
{
#pragma region A*計算
    //// 1. A*の再計算 (頻度を調整することを推奨)
    //pathRecalcTimer_ -= deltaTime;
    //if (pathRecalcTimer_ <= 0.0f)
    //{
    //    // プレイヤーの位置が変わった、または一定時間経過した場合のみ再計算
    //    // ここでの目標は「プレイヤーの現在地」ではなく「プレイヤーのいるグリッドノード」とします
    //    currentPath_ = FindPath(transform_.pos, player_.GetTransform().pos);
    //    currentPathIndex_ = 0;
    //    pathRecalcTimer_ = 0.5f; // 0.3秒から0.5秒程度に増やし、負荷を軽減
    //}

    //// 2. 経路が存在しない場合
    //if (currentPath_.empty() || currentPathIndex_ >= currentPath_.size())
    //{
    //    moveDir_ = AsoUtility::VECTOR_ZERO;
    //    // 経路が見つからなかった場合、その場で回転するなどの処理を追加できます
    //    return;
    //}

    //// 3. 移動目標の設定と到達判定
    //VECTOR targetPos = currentPath_[currentPathIndex_];

    //VECTOR currentPos = transform_.pos;
    //currentPos.y = targetPos.y; // 目標ノードのY座標に合わせる

    //VECTOR moveVector = VSub(targetPos, currentPos);
    //float distance = VSize(moveVector);

    //// ノードに到達したら次のノードへ
    //// 判定距離を小さくする（グリッドのサイズによって調整が必要）
    //// 例: グリッドサイズが200.0fの場合、20.0fは大きすぎる
    //// 5.0f～10.0f程度を推奨
    //constexpr float NODE_REACH_THRESHOLD = 10.0f; // 閾値を調整

    //if (distance < NODE_REACH_THRESHOLD)
    //{
    //    currentPathIndex_++;
    //    if (currentPathIndex_ >= currentPath_.size())
    //    {
    //        // 最終ノードに到達
    //        moveDir_ = AsoUtility::VECTOR_ZERO;
    //        return;
    //    }
    //    // 次のフレームで新しい目標ノードが設定される
    //}

#pragma endregion

    // 4. 移動方向と回転の設定
    // ノードの目標が更新されても、直前のノードから目標ノードへの移動方向を常に計算
    VECTOR moveDirection = VNorm(VSub(player_.GetTransform().pos, transform_.pos));
    moveDir_ = moveDirection;

    // moveSpeedはPATROL時より速い値を使うことが多い
    movePow_ = VScale(moveDir_, 4.0f);

    // 回転処理
    float rotationY = atan2f(moveDirection.x, moveDirection.z);
    Quaternion targetRotation = Quaternion::Euler({ 0.0f, rotationY, 0.0f });

    // 滑らかに補間
    // 補間速度をCHASEに合わせて調整
    transform_.quaRot = Quaternion::Slerp(transform_.quaRot, targetRotation, 7.0f * deltaTime); // 5.0fから7.0fに少し上げる


}

std::vector<VECTOR> EnemyBase::FindPath(VECTOR startPos, VECTOR endPos)
{
    if (!navGridManager_) return {};

    // 探索前のノードリセット
    navGridManager_->ResetAllNodes();

    AStarNode* startNode = navGridManager_->NodeFromWorldPoint(startPos);
    AStarNode* targetNode = navGridManager_->NodeFromWorldPoint(endPos);

    // 始点または終点が無効な場合
    if (!startNode || !targetNode || !targetNode->isWalkable_) {
        return {};
    }

    // A*リストの初期化
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNode> openList;
    std::unordered_set<AStarNode*> closedList; // 既に処理したノードを格納

    startNode->G_Score_ = 0;
    startNode->H_Score_ = GetHCost(startNode, targetNode);
    openList.push(startNode);

    while (!openList.empty()){
        AStarNode* currentNode = openList.top();
        openList.pop();

        // ノードが既に処理済みであればスキップ
        if (closedList.count(currentNode)) continue;

        closedList.insert(currentNode);

        // ゴールに到達
        if (currentNode == targetNode){
            return RetracePath(startNode, targetNode);
        }

        for (AStarNode* neighbor : GetNeighbors(currentNode)){
            if (!neighbor->isWalkable_ || closedList.count(neighbor)){
                continue;
            }

            float newGCost = currentNode->G_Score_ + GetDistance(currentNode, neighbor);

            if (newGCost < neighbor->G_Score_){
                neighbor->G_Score_ = newGCost;
                neighbor->H_Score_ = GetHCost(neighbor, targetNode);
                neighbor->parent_ = currentNode;

                // Fスコアが更新されたので、優先度キューに再投入
                openList.push(neighbor);
            }
        }
    }

    return {}; // 経路が見つからなかった
}

float EnemyBase::GetHCost(AStarNode* a, AStarNode* b)
{
    float dx = std::abs(static_cast<float>(a->gridX_) - static_cast<float>(b->gridX_));
    float dz = std::abs(static_cast<float>(a->gridZ_) - static_cast<float>(b->gridZ_));
    return dx + dz; // マンハッタン距離
}

std::vector<AStarNode*> EnemyBase::GetNeighbors(AStarNode* node)
{
    std::vector<AStarNode*> neighbors;

    // X, Z 方向のオフセット (8方向)
    int checkX[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int checkZ[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int i = 0; i < 8; ++i)
    {
        int neighborX = node->gridX_ + checkX[i];
        int neighborZ = node->gridZ_ + checkZ[i];

        if (navGridManager_ && neighborX >= 0 && neighborX < navGridManager_->GetGridSizeX() &&
            neighborZ >= 0 && neighborZ < navGridManager_->GetGridSizeZ()){
            AStarNode* neighborNode = navGridManager_->GetNode(neighborX, neighborZ);
            if (neighborNode){
                neighbors.push_back(neighborNode);
            }
        }
    }
    return neighbors;
}

float EnemyBase::GetDistance(AStarNode* a, AStarNode* b)
{
    VECTOR distVec = VSub(a->worldPos_, b->worldPos_);
    return VSize(distVec);
}

std::vector<VECTOR> EnemyBase::RetracePath(AStarNode* start, AStarNode* end)
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

void EnemyBase::HandleAttackCollsion(float deltaTime)
{
    if (!isDamageCheckActive_ || hasPlayerBeenHit_){
        return;
    }

    // 敵の現在のカプセル情報を取得
    const std::shared_ptr<Capsule> enemyCap = GetCapsule(); // EnemyBaseにGetCapsule()がある前提 (Charactor::GetCapsule()など)

    // プレイヤーの現在のカプセル情報を取得
    const std::shared_ptr<Capsule> playerCap = player_.GetCapsule(); // PlayerにGetCapsule()がある前提

    if (!enemyCap || !playerCap) {
        return;
    }

    // プレイヤーのカプセル座標を取得 (Player::GetCapsule()がTransformを考慮している前提)
    VECTOR pTop = playerCap->GetPosTop();
    VECTOR pDown = playerCap->GetPosDown();
    float pRadius = playerCap->GetRadius();

    // 敵のカプセル座標を取得
    VECTOR eTop = enemyCap->GetPosTop();
    VECTOR eDown = enemyCap->GetPosDown();
    float eRadius = enemyCap->GetRadius();

    VECTOR enemyCenter = VScale(VAdd(eTop, eDown), 0.5f);
    VECTOR playerCenter = VScale(VAdd(pTop, pDown), 0.5f);

    VECTOR diff = VSub(playerCenter, enemyCenter);
    float distanceSq = VSquareSize(diff);

    // 簡易的な衝突許容距離の二乗（半径の合計の二乗）
    // 敵のカプセル半径 + プレイヤーのカプセル半径 = 20.0f + 20.0f = 40.0f
    constexpr float COLLISION_RADIUS = 40.0f;
    constexpr float COLLISION_RADIUS_SQ = COLLISION_RADIUS * COLLISION_RADIUS;

    // 衝突判定
    if (distanceSq <= COLLISION_RADIUS_SQ){
        // 攻撃がヒットした
        player_.TakeDamage(30.0f);
        hasPlayerBeenHit_ = true;
    }
}

bool EnemyBase::isAttackRange(void)
{
    // プレイヤーの座標を取得
    VECTOR pPos = player_.GetTransform().pos;

    // エネミーからプレイヤーまでのベクトル
    VECTOR diff = VSub(pPos, transform_.pos);

    // 視野範囲に入っているか判断
    float distance = std::pow(diff.x, 2.0f) + std::pow(diff.z, 2.0f);
    if (distance <= (std::pow(ATTACK_RANGE, 2.0f))){
        // 自分から見たプレイヤーの角度を求める
        float rad = atan2(pPos.x - transform_.pos.x, pPos.z - transform_.pos.z);
        float viewRad = rad - transform_.rot.y;
        float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));

        // 視野角内に入っているか判断
        if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE)){
            auto info = MV1CollCheck_Line(player_.GetTransform().modelId, -1,
                { transform_.pos.x,transform_.pos.y,transform_.pos.z },
                { player_.GetTransform().pos.x,player_.GetTransform().pos.y,player_.GetTransform().pos.z });
            if (info.HitFlag == 0){
                return true;
            }
        }
    }

    return false;
}

bool EnemyBase::CompareNode::operator()(const AStarNode* a, const AStarNode* b) const
{
    if (a->F_Score() != b->F_Score()){
        // Fスコアが低い方を優先するため、降順ソート
        return a->F_Score() > b->F_Score();
    }
    // Fスコアが同じ場合、Hスコアが低い方を優先
    return a->H_Score_ > b->H_Score_;
}
