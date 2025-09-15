#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
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
	oxygen_ = AddComponent<OxygenComponent>(100.0f, 2.0f);
	input_ = AddComponent<PlayerInput>();

	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.rot = { 0.0f, 0.0f, 0.0f };
	transform_.scl = { 1.0f, 1.0f, 1.0f };

	transform_.Update();

	SetMousePoint(Application::GetInstance().GetWindowSize().width / 2,
		Application::GetInstance().GetWindowSize().height / 2);

	moveSpeed_ = 5.0f; // 移動速度
	yaw = 0.0f;       // 水平回転（ヨー）
	pitch = 0.0f;     // 垂直回転（ピッチ）
}

void Player::Update(float deltaTime)
{
	ActorBase::Update(deltaTime);
}

void Player::OnUpdate(float deltaTime)
{
    // === マウスの相対移動量を取得 ===
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    static int centerX = 640;  // 仮にウィンドウ中央
    static int centerY = 360;

    int deltaX = mouseX - centerX;
    int deltaY = mouseY - centerY;

    //// マウス位置を中央に戻す
    //SetMousePoint(centerX, centerY);

    // === 回転更新 ===
    yaw += deltaX * MOUSE_SENSITIVITY;  // 左右回転
    pitch += deltaY * MOUSE_SENSITIVITY;  // 上下回転

    // ピッチ制限（90度以上は見上げすぎない）
    if (pitch > DX_PI_F / 2.0f) pitch = DX_PI_F / 2.0f;
    if (pitch < -DX_PI_F / 2.0f) pitch = -DX_PI_F / 2.0f;

    // Quaternion更新（Yaw→Pitchの順）
    Quaternion qYaw = Quaternion::AngleAxis(yaw, AsoUtility::AXIS_Y);
    Quaternion qPitch = Quaternion::AngleAxis(pitch, AsoUtility::AXIS_X);
    transform_.quaRot = qYaw.Mult(qPitch);

    // === 入力で移動方向を決定 ===
    VECTOR forward = transform_.GetForward();
    VECTOR right = transform_.GetRight();
    VECTOR up = transform_.GetUp();

    VECTOR delta = VGet(0, 0, 0);

    if (CheckHitKey(KEY_INPUT_W)) delta = VAdd(delta, forward);
    if (CheckHitKey(KEY_INPUT_S)) delta = VAdd(delta, VScale(forward, -1.0f));
    if (CheckHitKey(KEY_INPUT_D)) delta = VAdd(delta, right);
    if (CheckHitKey(KEY_INPUT_A)) delta = VAdd(delta, VScale(right, -1.0f));
    if (CheckHitKey(KEY_INPUT_SPACE)) delta = VAdd(delta, up);
    if (CheckHitKey(KEY_INPUT_LSHIFT)) delta = VAdd(delta, VScale(up, -1.0f));

    if (VSize(delta) > 0.001f)
    {
        delta = VNorm(delta); // 正規化
        delta = VScale(delta, moveSpeed_ * deltaTime);
        transform_.pos = VAdd(transform_.pos, delta);
    }

    // Transform更新
    transform_.Update();


    // === カメラをプレイヤー位置に同期 ===
    SetCameraPositionAndTargetAndUpVec(
        transform_.pos,                       // カメラ位置
        VAdd(transform_.pos, forward),        // 注視点
        AsoUtility::DIR_U                     // 上方向
    );
}

void Player::Draw(void)
{
    int time = Application::GetInstance().GetDeltaTime();
	DrawFormatString(0, 0, GetColor(255, 255, 255), L"time: %.2f", time);
	DrawFormatString(0, 20, GetColor(255, 255, 255), L"pos=(%.2f,%.2f,%.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

OxygenComponent* Player::GetOxygenComp()
{
	return oxygen_;
}

PlayerInput* Player::GetInputComp()
{
	return input_;
}
