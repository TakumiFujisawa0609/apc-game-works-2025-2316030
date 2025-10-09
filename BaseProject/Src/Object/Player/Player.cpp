#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../Common/Collider.h"
#include "../Common/Capsule.h"
#include "../../Object/Components/Gameplay/OxygenComponent.h"
#include "../../Object/Components/Player/PlayerInput.h"
#include "Inventory.h"
#include "Player.h"

Player::Player(void)
    :
    oxygen_(nullptr),
    input_(nullptr),
    moveSpeed_(0.0f),
    moveDir_({}),
    movePow_({}),
    movedPos_({}),
    stepRotTime_(0.0f),
    gravHitPosDown_({}),
    gravHitPosUp_({}),
    jumpPow_({0.0f,30.0f,0.0f}),
    isJump_(false)
{
	
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// コンポーネントを生成してアタッチ
	oxygen_ = AddComponent<OxygenComponent>(100.0f, 1.0f);
	input_ = AddComponent<PlayerInput>();

	// モデル情報
	transform_.pos = { 0.0f, 200.0f, 0.0f };
	transform_.scl = { 1.0f, 1.0f, 1.0f };
    transform_.quaRot = Quaternion();
    transform_.quaRotLocal =
        Quaternion::Euler({ 0.0f,180.0f,0.0f });
	transform_.Update();

	SetMousePoint(Application::GetInstance().GetWindowSize().width / 2,
		Application::GetInstance().GetWindowSize().height / 2);

	moveSpeed_ = MOVE_WALK_SPEED; // 移動速度
    moveDir_ = AsoUtility::VECTOR_ZERO; // 移動方向
    movedPos_ = transform_.pos;

    playerRotY_ = Quaternion();
    goalQuaRot_ = Quaternion();
    stepRotTime_ = 0.0f;

    yaw = 0.0f;       // 水平回転（ヨー）
	pitch = 0.0f;     // 垂直回転（ピッチ）

    // カプセルコライダ
    capsule_ = std::make_shared<Capsule>(transform_);
    capsule_->SetLocalPosTop({ 0.0f,50.0f,0.0f });
    capsule_->SetLocalPosDown({ 0.0f,-200.0f,0.0f });
    capsule_->SetRadius(40.0f);

}

void Player::Update(float deltaTime)
{
	ActorBase::Update(deltaTime);

    oxygen_->GetOxygen();
}

void Player::OnUpdate(float deltaTime)
{
    // カメラの角度を取得
    VECTOR angles = Application::GetInstance().GetCamera()->GetAngles();

    // Quaternion更新
    Quaternion qYaw = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles.y), AsoUtility::AXIS_Y);
    Quaternion qPitch = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles.x), AsoUtility::AXIS_X);
    
    // プレイヤーの回転をカメラと同期
    transform_.quaRot = qYaw.Mult(qPitch);

    // === 入力で移動方向を決定 ===

    // カメラ情報取得
    auto camera = Application::GetInstance().GetCamera();

    // 前方方向
    VECTOR forward = camera->GetForward();
    // 右方向
    VECTOR right = camera->GetRight();
    // 上方向
    VECTOR up = camera->GetUp();

    // 重力方向に垂直なカメラ角度(XZ平面)を取得
    Quaternion cameraRot = Application::GetInstance().GetCamera()->GetQuaRotOutX();

    // 回転したい角度
    double rotRad = 0.0;
    
    // 移動方向のリセット
    moveDir_ = AsoUtility::VECTOR_ZERO;

    // 入力関連
    auto& ins = InputManager::GetInstance();
    
    // 前後左右入力
    if (ins.IsNew(KEY_INPUT_W)) moveDir_ = VAdd(moveDir_, forward);
    if (ins.IsNew(KEY_INPUT_S)) moveDir_ = VAdd(moveDir_, VScale(forward, -1.0f));
    if (ins.IsNew(KEY_INPUT_D)) moveDir_ = VAdd(moveDir_, right);
    if (ins.IsNew(KEY_INPUT_A)) moveDir_ = VAdd(moveDir_, VScale(right, -1.0f));

    //if (ins.IsNew(KEY_INPUT_SPACE)) moveDir_ = VAdd(moveDir_, up);
    //if (ins.IsNew(KEY_INPUT_LSHIFT)) moveDir_ = VAdd(moveDir_, VScale(up, -1.0f));

    if (ins.IsNew(KEY_INPUT_LSHIFT))
    {
        moveSpeed_ = MOVE_RUN_SPEED;
    }
    else
    {
        moveSpeed_ = MOVE_WALK_SPEED;
    }

    if (!AsoUtility::EqualsVZero(moveDir_))
    {
        moveDir_ = VNorm(moveDir_);
        rotRad = std::atan2(moveDir_.x, moveDir_.z);

        // 正規化して移動量を算出
        VECTOR direction = VNorm(transform_.quaRot.PosAxis(moveDir_));

        // 移動量
        movePow_ = VScale(direction, moveSpeed_);
        
        // 回転処理
        SetGoalRotate(rotRad);

        // 重力計算
        CalcGravityPow();

        movedPos_ = VAdd(transform_.pos, movePow_);
        Collision();
        transform_.pos = movedPos_;
    }
    
    // Transform更新
    transform_.Update();
}

void Player::Draw(void)
{
	DrawFormatString(0, 20, GetColor(255, 255, 255), L"pos=(%.2f,%.2f,%.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
    DrawFormatString(0, 36, GetColor(255, 255, 255), L"quaRot=(%.2f,%.2f,%.2f)", transform_.quaRot.x, transform_.quaRot.y, transform_.quaRot.z);
}

OxygenComponent* Player::GetOxygenComp()
{
	return oxygen_;
}

PlayerInput* Player::GetInputComp()
{
	return input_;
}

bool Player::TakeItem(int itemId, int count)
{
  //  if (inventory_->HasItem(itemId) &&
  //      inventory_->GetItemCount(itemId) >= count)
  //  {
		//inventory_->RemoveItem(itemId, count);
		//return true;
  //  }
    return false;
}

void Player::AddCollider(std::shared_ptr<Collider> collider)
{
    colliders_.push_back(collider);
}

void Player::ClearCollider(void)
{
    colliders_.clear();
}

const std::shared_ptr<Capsule> Player::GetCapsule(void) const
{
    return capsule_;
}

void Player::SetGoalRotate(float rotRad)
{
    VECTOR angeles = Application::GetInstance().GetCamera()->GetAngles();
    double cameraRadY = AsoUtility::Deg2RadD(angeles.y);

    Quaternion axis = Quaternion::AngleAxis(cameraRadY + rotRad, AsoUtility::AXIS_Y);

    // 現在設定されている回転との角度差を取る
    double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

    // しきい値
    if (angleDiff > 0.1)
    {
        stepRotTime_ = 0.3f;
    }

    goalQuaRot_ = axis;
}

void Player::GiveItem(int itemId, int count)
{
	//inventory_->AddItem(itemId, count);
}

void Player::Collision(void)
{
    CollisionCapsule();

    CollisionGravity();
}

void Player::CollisionCapsule(void)
{
    // カプセルを移動させる
    Transform trans = Transform(transform_);
    trans.pos = movedPos_;
    trans.Update();
    Capsule cap = Capsule(*capsule_, trans);

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

void Player::CollisionGravity(void)
{
    // ジャンプ量を加算
    movedPos_ = VAdd(movedPos_, jumpPow_);

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

        if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
        {
            // 衝突地点から、少し上に移動
            movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

            // ジャンプリセット
            jumpPow_ = AsoUtility::VECTOR_ZERO;
            isJump_ = false;
        }
    }
}

void Player::CalcGravityPow(void)
{
    // 重力方向
    VECTOR dirGravity = AsoUtility::DIR_D;

    // 重力の強さ
    float gravityPow = 25.0f;

    // 重力
    VECTOR gravity = VScale(dirGravity, gravityPow);
    jumpPow_ = VAdd(jumpPow_, gravity);

    // 内積
    float dot = VDot(dirGravity, jumpPow_);
    if (dot >= 0.0f)
    {
        // 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
        jumpPow_ = gravity;
    }
}
