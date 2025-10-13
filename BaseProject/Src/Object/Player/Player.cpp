#include <DxLib.h>
#include <memory>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../Common/Collider.h"
#include "../Common/Capsule.h"
#include "Inventory.h"
#include "Player.h"
#include "../../Object/Components/Gameplay/OxygenComponent.h"
#include "../../Object/Components/Player/PlayerInput.h"

Player::Player(void)
    :
    oxygen_(nullptr),
    input_(nullptr)
{
	
}

Player::~Player(void)
{
}

void Player::Init(void)
{
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

    startRotY_ = Quaternion();
    goalQuaRot_ = Quaternion();
    stepRotTime_ = 0.0f;

    yaw = 0.0f;       // 水平回転（ヨー）
	pitch = 0.0f;     // 垂直回転（ピッチ）

    // カプセルコライダ
    capsule_ = std::make_shared<Capsule>(transform_);
    capsule_->SetLocalPosTop({ 0.0f,50.0f,0.0f });
    capsule_->SetLocalPosDown({ 0.0f,-400.0f,0.0f });
    capsule_->SetRadius(40.0f);

}

void Player::InitComponents(void)
{
    // コンポーネントを生成してアタッチ
    oxygen_ = AddCharactorComponent<OxygenComponent>(100.0f, 1.0f);
    input_ = AddCharactorComponent<PlayerInput>();
}

void Player::Update(float deltaTime)
{
	Charactor::Update(deltaTime);

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
        transform_.pos.y = 189.0f;
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
    return false;
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

    /*if (transform_.pos.y > 189.0f)
    {*/
        CollisionGravity();
	//}
    //else
    //{
    //    // 地面に到達したらy座標を固定
    //    transform_.pos.y = 189.0f;
    //    velocityY_ = AsoUtility::VECTOR_ZERO;
    //    isVelocityY_ = false;
    //}
}

void Player::CollisionCapsule(void)
{
    Charactor::CollisionCapsule();
}

void Player::CollisionGravity(void)
{
    Charactor::CollisionGravity();
}

void Player::CalcGravityPow(void)
{
    Charactor::CalcGravityPow();
}
