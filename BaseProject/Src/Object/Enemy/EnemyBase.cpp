#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Collider.h"
#include "../Common/Capsule.h"
#include "../Common/AnimationController.h"
#include "../Components/Charactor/Enemy/EnemyMoveComponent.h"
#include "../Components/Charactor/Enemy/EnemyChaseComponent.h"

#include "Patrol/PatrolNode.h"
#include "Patrol/PatrolPath.h"
#include "EnemyBase.h"

const std::map<EnemyBase::STATE, const char*>stateNames =
{
    {EnemyBase::STATE::PATROL,"PATROL"},
    {EnemyBase::STATE::CHASE,"CHASE"},
    {EnemyBase::STATE::ATTACK,"ATTACK"},
    {EnemyBase::STATE::IDLE,"IDLE"}
};

EnemyBase::EnemyBase(Player& player)
    :
    player_(player),
    moveComponent_(nullptr),
    chaseComponent_(nullptr),
    isHearingSound_(false),
    isPlayerInSight_(false),
    currentPatrolPathIndex_(-1),
    state_(STATE::IDLE)
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

    // 移動コンポーネント
    moveComponent_ = AddCharactorComponent<EnemyMoveComponent>(player_);

    // 追跡コンポ―ネント
    chaseComponent_ = AddCharactorComponent<EnemyChaseComponent>(player_);

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


    if (isPlayerInSight_ || isHearingSound_)
    {
        // 追跡状態への遷移条件
        if (state_ != STATE::CHASE)
        {
            state_ = STATE::CHASE;
        }
    }
    else
    {
        // 巡回状態に復帰
        if (state_ == STATE::CHASE)
        {
            state_ = STATE::PATROL;
        }
    }

    // 現在の状態に応じたコンポーネントのロジック実行
    switch (state_)
    {
    case EnemyBase::STATE::PATROL:
        if (moveComponent_)
        {
            // ここで巡回情報をわたす
            moveComponent_->Patrol(deltaTime, transform_, patrolPath_, currentPatrolPathIndex_,
                moveDir_, movePow_, 5.0f, transform_.quaRot);

        }
        break;
    case EnemyBase::STATE::CHASE:
        if (chaseComponent_)
        {
            // 追跡処理時に必要な情報を渡す
            chaseComponent_->Chase(deltaTime);
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
    movePow_ = VAdd(movePow_, horizontalMovement);

    CalcGravityPow();

    Collision();
}

void EnemyBase::Draw(void)
{
    MV1DrawModel(transform_.modelId);

#ifdef _DEBUG

    const char* stateName = "UNKONOWN";
    auto it = stateNames.find(state_);
    if (it != stateNames.end())
    {
        stateName = it->second;
    }

    DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 48, GetColor(255, 255, 255), L"state: %s", stateName);

    if (capsule_)capsule_->Draw();

    DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 64, GetColor(255, 255, 255), L"pos: %.2f,%.2f,%.2f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
    DrawFormatString(Application::GetInstance().GetWindowSize().width - 150, 80, GetColor(255, 255, 255), L"dis: %.2f", moveComponent_->GetDis());

#endif // _DEBUG


}

void EnemyBase::SetObstacle(std::vector<std::shared_ptr<Transform>> obstacle)
{
    chaseComponent_->SetObstacle(obstacle);
}

void EnemyBase::SetPatrolPath(std::shared_ptr<PatrolPath> path)
{
    patrolPath_ = path;
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
