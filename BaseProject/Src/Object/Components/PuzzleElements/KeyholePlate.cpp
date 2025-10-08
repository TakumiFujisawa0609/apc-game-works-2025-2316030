#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/ResourceManager.h"
#include "KeyholePlate.h"

KeyholePlate::KeyholePlate(std::shared_ptr<ActorBase> owner)
	:
	Component(owner),
	resMng_(ResourceManager::GetInstance()),
	imgH_(-1),
	pos_({}),
	extRate_(0.0f),
	angle_(0.0f)
{

}

KeyholePlate::~KeyholePlate(void)
{
}

void KeyholePlate::Init(void)
{
	// 画像の初期化
	imgH_ = resMng_.Load(ResourceManager::SRC::E_PLATE_I).handleId_;

	// 画像表示の中心座標
	pos_.x = Application::GetInstance().GetWindowSize().width / 2;
	pos_.y = Application::GetInstance().GetWindowSize().height / 2;

	// 拡大率
	extRate_ = 0.5f;

	// 角度
	angle_ = 0.0f;
}

void KeyholePlate::Update(float deltaTime)
{
}

void KeyholePlate::Draw(void)
{
	DrawRotaGraph(
		pos_.x,
		pos_.y,
		extRate_,
		angle_,
		imgH_,
		true,
		false);
}
