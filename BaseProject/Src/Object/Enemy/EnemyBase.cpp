#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Capsule.h"
#include "../Components/Enemy/EnemyMoveComponent.h"
#include "../Components/Enemy/EnemyChaseComponent.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
    :
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

    // 状態の初期化

    // 移動コンポーネント
    moveComponent_ = AddCharactorComponent<EnemyMoveComponent>();

    // 追跡コンポ―ネント
    chaseComponent_ = AddCharactorComponent<EnemyChaseComponent>();

    // モデルの更新
    transform_.Update();
}

void EnemyBase::InitComponents(void)
{
}

void EnemyBase::Update(float deltaTime)
{

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
