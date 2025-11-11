#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "Charactor.h"

Charactor::Charactor(void)
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
	velocityY_({ 0.0f,30.0f,0.0f }),
	isVelocityY_(false)
{
    type_ = ActorBase::TYPE::SKINING;
    capsule_ = std::make_unique<Capsule>(&transform_);
}

Charactor::~Charactor()
{
	transform_.Release();
}

void Charactor::Update(float deltaTime)
{
    for (auto& comp : charaComponents_)
    {
        comp->Update(deltaTime);
    }
    OnUpdate(deltaTime);
}

void Charactor::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Charactor::ClearCollider(void)
{
	colliders_.clear();
}

void Charactor::InitializeCapsule(VECTOR top, VECTOR down, float radius)
{
    capsule_->SetLocalPosTop(top);
    capsule_->SetLocalPosDown(down);
    capsule_->SetRadius(radius);
}

const std::shared_ptr<Capsule> Charactor::GetCapsule(void) const
{
	return capsule_;
}

void Charactor::Rotate(void)
{
	stepRotTime_ -= Application::GetInstance().GetDeltaTime();

	// 回転の球面補間
	startRotY_ = Quaternion::Slerp(
		startRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Charactor::Collision(void)
{
	// カプセルとの当たり判定
	//CollisionCapsule();

	// 重力の衝突判定
	//CollisionGravity();
}

void Charactor::CollisionCapsule(void)
{
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
                    movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 0.01f));

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

void Charactor::CollisionGravity(void)
{
    // ジャンプ量を加算
    movedPos_ = VAdd(movedPos_, velocityY_);

    // 重力方向
    VECTOR dirGravity = AsoUtility::DIR_D;

    // 重力方向の反対
    VECTOR dirUpGravity = AsoUtility::DIR_U;

    // 重力の強さ
    float gravityPow = 25.0f;

    // 頭上判定時の押出力
    float checkPow = 10.0f;
    gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));

    //天井衝突チェックのために広めに2.0fをかけて判定をとる
    gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
    gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
    for (const auto& c : colliders_)
    {
        // 地面との衝突
        auto hit = MV1CollCheck_Line(
            c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

        if (hit.HitFlag > 0 && VDot(dirGravity, velocityY_) > 0.9f)
        {
            // 衝突地点から、少し上に移動
            movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

            // ジャンプリセット
            velocityY_ = AsoUtility::VECTOR_ZERO;
            isVelocityY_ = false;
        }
    }
}

void Charactor::CalcGravityPow(void)
{
    // 重力方向
    VECTOR dirGravity = AsoUtility::DIR_D;

    // 重力の強さ
    float gravityPow = 25.0f;

    // 重力
    VECTOR gravity = VScale(dirGravity, gravityPow);
    velocityY_ = VAdd(velocityY_, gravity);

    // 内積
    float dot = VDot(dirGravity, velocityY_);
    if (dot >= 0.0f)
    {
        // 重力方向と反対方向(マイナス)でなければ、Y軸方向の移動変化量を無くす
        velocityY_ = gravity;
    }
}


