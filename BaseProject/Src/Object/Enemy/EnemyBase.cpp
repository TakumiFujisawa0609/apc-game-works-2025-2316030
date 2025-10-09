#include "../../Application.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
    :
    moveSpeed_(0.0f),
    moveDir_({}),
    movePow_({}),
    movedPos_({}),
    startRotY_({}),
    goalQuaRot_({}),
    stepRotTime_(0.0f),
    gravHitPosDown_({}),
    gravHitPosUp_({}),
    jumpPow_({ 0.0f,30.0f,0.0f }),
    isJump_(false)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(void)
{
}

void EnemyBase::Update(float deltaTime)
{


    // ÉÇÉfÉãÇÃçXêV
    transform_.Update();
}

void EnemyBase::OnUpdate(float deltaTime)
{
}

void EnemyBase::Draw(void)
{
    MV1DrawModel(transform_.modelId);
}

void EnemyBase::AddCollider(std::shared_ptr<Collider> collider)
{
    colliders_.push_back(collider);
}

void EnemyBase::ClearCollider(void)
{
    colliders_.clear();
}

const std::shared_ptr<Capsule> EnemyBase::GetCapsule(void) const
{
    return capsule_;
}

void EnemyBase::Rotate(void)
{
    stepRotTime_ -= Application::GetInstance().GetDeltaTime();

    // âÒì]ÇÃãÖñ ï‚ä‘
    startRotY_ = Quaternion::Slerp(
        startRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void EnemyBase::Collision(void)
{

    CollisionCapsule();

    CollisionGravity();
}

void EnemyBase::CollisionCapsule(void)
{

}

void EnemyBase::CollisionGravity(void)
{
}

void EnemyBase::CalcGravityPow(void)
{
}
