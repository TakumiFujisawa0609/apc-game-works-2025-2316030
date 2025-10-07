#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/ResourceManager.h"
#include "Keyhole.h"

Keyhole::Keyhole(std::shared_ptr<ActorBase> owner)
	:
	Component(owner),
	resMng_(ResourceManager::GetInstance()),
	imgH_(-1),
	pos_({}),
	extRate_(0.0f),
	angle_(0.0f)
{
}

Keyhole::~Keyhole(void)
{
}

void Keyhole::Init(void)
{
	// 画像初期化
	imgH_ = resMng_.Load(ResourceManager::SRC::KEYHOLE_I).handleId_;

	// 画像表示の中心座標
	pos_.x = Application::GetInstance().GetWindowSize().width / 2;
	pos_.y = Application::GetInstance().GetWindowSize().height / 2;

	// 拡大率
	extRate_ = 0.5f;

	// 角度
	angle_ = 0.0f;

}

void Keyhole::Update(float deltaTime)
{
}

void Keyhole::Draw(void)
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

void Keyhole::SetAngle(float angle)
{
	angle_ = angle_;
}
