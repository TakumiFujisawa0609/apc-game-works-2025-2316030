#include "OverScene.h"
#include<DxLib.h>
#include<cassert>
#include"../Manager/SceneController.h"
#include"../Input.h"
#include"TitleScene.h"


namespace {
	constexpr int fade_interval = 60;
}

void OverScene::FadeInUpadte(Input&)
{
	if (--frame_ <=0) {
		update_ = &OverScene::NormalUpadte;
		draw_ = &OverScene::NormalDraw;
	}
}

void OverScene::NormalUpadte(Input& input)
{
	if (input.IsTriggered("ok")) {
		frame_ = 0;
		update_ = &OverScene::FadeOutUpadte;
		draw_ = &OverScene::FadeDraw;
	}
}

void OverScene::FadeOutUpadte(Input& input)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_),input);
		return;
	}
}

void OverScene::NormalDraw()
{
	//DrawRotaGraph(320, 240, 1.0f, 0.0f, imgH_, true);
	DrawString(10, 10, L"Gameover Scene", 0xffffff);
}

void OverScene::FadeDraw()
{
	float rate = static_cast<float>(frame_) /
					static_cast<float>(fade_interval);	
	//DrawRotaGraph(320, 240, 1.0f, 0.0f, imgH_, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

OverScene::OverScene(SceneController& controller) :Scene(controller)
{
	//imgH_ = LoadGraph(L"img/hasuta.png");
	//assert(imgH_ >= 0);
	update_ = &OverScene::FadeInUpadte;
	draw_ = &OverScene::FadeDraw;
	frame_ = fade_interval;
}

OverScene::~OverScene()
{
	DeleteGraph(imgH_);
}

void OverScene::Init(Input& input)
{
}

void OverScene::Update(Input& input)
{
	(this->*update_)(input);
}

void OverScene::Draw()
{
	(this->*draw_)();
}
