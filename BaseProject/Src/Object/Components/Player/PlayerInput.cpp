#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/InputManager.h"
#include "PlayerInput.h"

PlayerInput::PlayerInput(std::shared_ptr<Charactor> owner)
	: 
	CharactorComponent(owner),
	moveForward_(0.0f),
	moveRight_(0.0f),
	mouseDeltaX_(0.0f),
	mouseDeltaY_(0.0f)
{
	// マウス初期位置を画面中央にセット
	SetMousePoint(
		Application::GetInstance().GetWindowSize().width / 2,
		Application::GetInstance().GetWindowSize().height / 2);
}

PlayerInput::~PlayerInput(void)
{
}

void PlayerInput::Update(float deltaTime)
{
	auto& ins = InputManager::GetInstance();
	// 前後左右入力
	if (ins.IsNew(KEY_INPUT_W)) moveForward_ += 1.0f;
	if (ins.IsNew(KEY_INPUT_S)) moveForward_ -= 1.0f;
	if (ins.IsNew(KEY_INPUT_D)) moveRight_ += 1.0f;
	if (ins.IsNew(KEY_INPUT_A)) moveRight_ -= 1.0f;

	// マウス移動量取得
	int mx, my;
	GetMousePoint(&mx, &my);

	mouseDeltaX_ = static_cast<float>(mx - Application::GetInstance().GetWindowSize().width / 2);
	mouseDeltaY_ = static_cast<float>(my - Application::GetInstance().GetWindowSize().height / 2);
}

float PlayerInput::GetMoveForward(void) const
{
    return moveForward_;
}

float PlayerInput::GetMoveRight(void) const
{
    return moveRight_;
}

float PlayerInput::GetMouseDeltaX(void) const
{
    return mouseDeltaX_;
}

float PlayerInput::GetMouseDeltaY(void) const
{
    return mouseDeltaY_;
}
