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
    currentNode_(0)
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

    if (EyeSerch() || isHearingSound_)
    {
        // 追跡状態への遷移条件
        if (state_ != STATE::CHASE)
        {
            ChangeState(STATE::CHASE);
        }
        isChase_ = true;
        frame_ = 0;
    }
    else
    {
        isChase_ = false;
    }

    if (!isChase_)
    {
        frame_++;
        if (frame_ >= 180)
        {
            ChangeState(STATE::PATROL);
        }
    }

    // 現在の状態に応じたコンポーネントのロジック実行
    switch (state_)
    {
    case EnemyBase::STATE::PATROL:
        UpdatePatrol(deltaTime);
        //if (patrolComponent_)
        //{
        //    // ここで巡回情報をわたす
        //    patrolComponent_->Patrol(deltaTime, transform_, patrolPath_, currentPatrolPathIndex_,
        //        moveDir_, movePow_, 5.0f, transform_.quaRot);

        //}
        break;
    case EnemyBase::STATE::CHASE:
        UpdateChase(deltaTime);
        if (chaseComponent_)
        {   
            // 追跡処理時に必要な情報を渡す
            chaseComponent_->Chase(deltaTime, transform_,
                moveDir_, movePow_, 5.0f, transform_.quaRot);
        }
        break;
    case EnemyBase::STATE::ATTACK:

        // 一定距離まで近づくと攻撃されて
        // プレイヤーがダメージを受ける

        break;
    case EnemyBase::STATE::IDLE:
        break;
    default:
        break;
    }


    // 移動
    VECTOR horizontalMovement = VScale(moveDir_, moveSpeed_ * deltaTime);
    //movePow_ = VAdd(movePow_, horizontalMovement);

    //CalcGravityPow();

    //Collision();
}

void EnemyBase::Draw(void)
{
    MV1DrawModel(transform_.modelId);

#ifdef _DEBUG

    const wchar_t* stateName = L"UNKONOWN";
    auto it = stateNames.find(state_);
    if (it != stateNames.end())
    {
        stateName = it->second;
    }

    DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 48, GetColor(255, 255, 255), L"state: %s", stateName);

    if (capsule_)capsule_->Draw();

    DrawDebug();

    DrawFormatString(Application::GetInstance().GetWindowSize().width - 200, 64, GetColor(255, 255, 255), L"pos: (%.2f,%.2f,%.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
    DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 80, GetColor(255, 255, 255), L"dis: %.2f", patrolComponent_->GetDis());
    DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 96, GetColor(255, 255, 255), L"nextNode: %d", patrolComponent_->GetCurrentNode());
    DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 112, GetColor(255, 255, 255), L"recalcTime: %.2f", chaseComponent_->GetTime());

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

    Charactor::Collision();

    // 移動
    transform_.pos = movedPos_;
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
    for (const auto& c : colliders_)
    {
        auto hits = MV1CollCheck_Capsule(
            c->modelId_, -1,
            cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

        for (int i = 0; i < hits.HitNum; i++)
        {
            auto hit = hits.Dim[i];
            for (int tryCnt = 0; tryCnt < 10; tryCnt++)
            {
                int pHit = HitCheck_Capsule_Triangle(
                    cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
                    hit.Position[0], hit.Position[1], hit.Position[2]);
                if (pHit)
                {
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

    switch (state_)
    {
    case EnemyBase::STATE::PATROL:
        animationController_->Play((int)ANIM::PATROL, true, 1.0f);
        break;
    case EnemyBase::STATE::CHASE:
        animationController_->Play((int)ANIM::CHASE, true, 0.5f);
        break;
    case EnemyBase::STATE::ATTACK:
        animationController_->Play((int)ANIM::ATTACK, true, 0.5f);
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
    if (distance <= (std::pow(EYE_VIEW_RANGE, 2.0f)))
    {
        // 自分から見たプレイヤーの角度を求める
        float rad = atan2(pPos.x - transform_.pos.x, pPos.z - transform_.pos.z);
        float viewRad = rad - transform_.rot.y;
        float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));

        // 視野角内に入っているか判断
        if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
        {
            auto info = MV1CollCheck_Line(player_.GetTransform().modelId, -1,
                { transform_.pos.x,transform_.pos.y,transform_.pos.z },
                { player_.GetTransform().pos.x,player_.GetTransform().pos.y,player_.GetTransform().pos.z });
            if (info.HitFlag == 0)
            {
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
    if (isWaiting_)
    {
        currentWaitTime_ -= deltaTime;

        if (currentWaitTime_ <= 0.0f)
        {
            // 待機終了。次のノードへ進む
            isWaiting_ = false;

            // 状態をPATROL（移動）に戻す
            //enemyBase->ChangeState(EnemyBase::STATE::PATROL);
        }
        else
        {
            // 次のノードのインデックスを更新
            patrolPath_->GetNextNode(currentPatrolPathIndex_);
            currentNode_ = currentPatrolPathIndex_;

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
    if (distance < 5.5f)
    {
        // 目標に到達したら待機状態に遷移
        isWaiting_ = true;
        currentWaitTime_ = targetNode.GetWaitTime();

        // 移動を停止
        moveDir_ = AsoUtility::VECTOR_ZERO;
    }
    else
    {
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

}
