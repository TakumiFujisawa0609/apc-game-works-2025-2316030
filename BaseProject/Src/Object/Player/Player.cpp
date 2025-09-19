#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Object/Components/Gameplay/OxygenComponent.h"
#include "../../Object/Components/Player/PlayerInput.h"
#include "Player.h"

Player::Player(void)
{
	
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	//oxygen_ = std::make_shared<OxygenComponent>();
	
	// コンポーネントを生成してアタッチ
	oxygen_ = AddComponent<OxygenComponent>(100.0f, 1.0f);
	input_ = AddComponent<PlayerInput>();

	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.rot = { 0.0f, 0.0f, 0.0f };
	transform_.scl = { 1.0f, 1.0f, 1.0f };

	transform_.Update();

	SetMousePoint(Application::GetInstance().GetWindowSize().width / 2,
		Application::GetInstance().GetWindowSize().height / 2);

	moveSpeed_ = 5.0f; // 移動速度
    moveDir_ = AsoUtility::VECTOR_ZERO; // 移動方向
    movedPos_ = transform_.pos;

    playerRotY_ = Quaternion();
    goalQuaRot_ = Quaternion();
    stepRotTime_ = 0.0f;

    yaw = 0.0f;       // 水平回転（ヨー）
	pitch = 0.0f;     // 垂直回転（ピッチ）
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

    if (ins.IsNew(KEY_INPUT_SPACE)) moveDir_ = VAdd(moveDir_, up);
    if (ins.IsNew(KEY_INPUT_LSHIFT)) moveDir_ = VAdd(moveDir_, VScale(up, -1.0f));

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

        movedPos_ = VAdd(transform_.pos, movePow_);
        transform_.pos = movedPos_;
    }

    // 移動方向に応じた回転
    Rotate();

    // 回転させる
    transform_.quaRot = playerRotY_;
    
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

void Player::Rotate(void)
{
    stepRotTime_ -= Application::GetInstance().GetDeltaTime();

    // 回転の球面補間
    playerRotY_ = Quaternion::Slerp(
        playerRotY_, goalQuaRot_, (0.3f - stepRotTime_) / 0.3f);
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
