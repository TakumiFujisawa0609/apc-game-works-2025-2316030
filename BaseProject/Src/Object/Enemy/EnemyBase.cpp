#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Capsule.h"
#include "../Common/AnimationController.h"
#include "../Components/Charactor/Enemy/EnemyMoveComponent.h"
#include "../Components/Charactor/Enemy/EnemyChaseComponent.h"

#include "Patrol/PatrolPath.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(Player& player)
    :
    player_(player),
    //moveComponent_(nullptr),
    //chaseComponent_(nullptr),
    isHearingSound_(false),
    isPlayerInSight_(false)
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


    // 状態の初期化
    state_ = STATE::IDLE;
    
    currentPatrolPathIndex_ = 0;

    // 移動コンポーネント
    moveComponent_ = AddCharactorComponent<EnemyMoveComponent>(player_);

    // 追跡コンポ―ネント
    chaseComponent_ = AddCharactorComponent<EnemyChaseComponent>(player_);

    InitAnimation();

    // モデルの更新
    transform_.Update();
}

void EnemyBase::InitComponents(void)
{
}

void EnemyBase::Update(float deltaTime)
{
    if (isPlayerInSight_ || isHearingSound_)
    {
        // 追跡状態への遷移条件
        if (state_ != STATE::CAHSE)
        {
            state_ = STATE::CAHSE;
        }
    }
    else
    {
        // 巡回状態に復帰
        if (state_ == STATE::CAHSE)
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
        
            moveComponent_->Patrol(patrolPath_, currentPatrolPathIndex_, deltaTime);
        }
        break;
    case EnemyBase::STATE::CAHSE:
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

    // モデルの更新
    transform_.Update();
}

void EnemyBase::OnUpdate(float deltaTime)
{
}

void EnemyBase::Draw(void)
{
    MV1DrawModel(transform_.modelId);
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
    animationController_ = std::make_unique<AnimationController>();

}

void EnemyBase::Rotate(void)
{
    Charactor::Rotate();
}

void EnemyBase::Collision(void)
{
    Charactor::Collision();
}

void EnemyBase::CollisionCapsule(void)
{
    Charactor::CollisionCapsule();
}

void EnemyBase::CollisionGravity(void)
{
    Charactor::CollisionGravity();
}

void EnemyBase::CalcGravityPow(void)
{
    Charactor::CalcGravityPow();
}
