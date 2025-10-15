#include "TitleScene.h"
#include<DxLib.h>
#include<memory>
#include<cassert>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include"../Manager/SceneController.h"
#include"../Manager/ResourceManager.h"
#include"GameScene.h"
#include"../Input.h"
namespace {
	constexpr int fade_interval = 60;
}

TitleScene::TitleScene(SceneController& controller)
	:
	Scene(controller),
	titleH_(-1)
{
	//titleH_=LoadGraph(L"img/fukuro.png");
	//assert(titleH_);

}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(Input& input)
{
	update_ = &TitleScene::FadeInUpdate;
	draw_ = &TitleScene::FadeDraw;
	frame_ = fade_interval;
	titleH_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	soundH_ = resMng_.Load(ResourceManager::SRC::DECIDE_SE).handleId_;
	ChangeVolumeSoundMem(255, soundH_);
}

void TitleScene::Update(Input& input)
{
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	(this->*draw_)();
}

void TitleScene::FadeInUpdate(Input&)
{
	--frame_;
	if (frame_ <= 0) {
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	auto& ins = InputManager::GetInstance();

	if (input.IsTriggered("ok") || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) {
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		frame_ = 0;
		PlaySoundMem(soundH_, DX_PLAYTYPE_NORMAL, true);
		return;
	}

}

void TitleScene::FadeOutUpdate(Input& input)
{
	if (++frame_ >= fade_interval) {//遷移条件
		controller_.ChangeScene(std::make_shared<GameScene>(controller_), input);
		return;//シーンを入れ替えたら処理を打ち切る
	}
}

void TitleScene::FadeDraw()
{
	//DrawRotaGraph(320, 240, 1.0f, 0.0f, titleH_, true);

	//フェードしつつ0～1の範囲の値の「割合」を計算している
	float rate = static_cast<float>(frame_) / 
					static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(rate * 255));
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{

	auto size = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(size.width / 2, static_cast<int>(size.height / 2 * 0.8f), 0.4, 0.0, titleH_, true);


	const TCHAR* text_to_display = _T("Space or Aボタン");
	int text_width = GetDrawStringWidth(text_to_display, _tcslen(text_to_display));

	// X座標: 画面中央 (画面幅 / 2) からテキスト幅の半分を引く
	int draw_x = (size.width / 2) - (text_width / 2);

	// Y座標: 画面全体の高さの 4分の3 の位置
	int draw_y = (size.height * 3) / 4;

	// 3. テキストを描画

	// 赤色で描画
	int color = GetColor(255, 255, 255); // 白にする場合は GetColor(255, 255, 255)

	// 描画関数でテキストを表示
	DrawString(draw_x, draw_y, text_to_display, color);
	//DrawString(10, 10, L"Title Scene", 0xffffff);
}
