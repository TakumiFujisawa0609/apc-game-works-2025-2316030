#include "ClearScene.h"
#include<DxLib.h>
#include<cassert>
#include "../Application.h"
#include"../Manager/SceneController.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Input.h"
#include"TitleScene.h"

namespace {
	constexpr int fade_interval = 30;
}

ClearScene::ClearScene(SceneController& controller)
	:
	Scene(controller)
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init(Input& input)
{
	// クリア画像を初期化
		//imgH_ = LoadGraph(L"img/hasuta.png");
	//assert(imgH_ >= 0);
	update_ = &ClearScene::FadeInUpadte;
	draw_ = &ClearScene::FadeDraw;
	frame_ = fade_interval;
	imgH_ = resMng_.Load(ResourceManager::SRC::GAMECLEAR).handleId_;
	soundH_ = resMng_.Load(ResourceManager::SRC::DECIDE_SE).handleId_;
	ChangeVolumeSoundMem(255, soundH_);
}

void ClearScene::Update(Input& input)
{
	(this->*update_)(input);
}

void ClearScene::Draw(void)
{
	(this->*draw_)();
}

void ClearScene::FadeInUpadte(Input& input)
{
	if (--frame_ <= 0) {
		update_ = &ClearScene::NormalUpadte;
		draw_ = &ClearScene::NormalDraw;
	}
}

void ClearScene::NormalUpadte(Input& input)
{
	auto& ins = InputManager::GetInstance();

	if (input.IsTriggered("ok") || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) {
		frame_ = 0;
		update_ = &ClearScene::FadeOutUpadte;
		draw_ = &ClearScene::FadeDraw;
		PlaySoundMem(soundH_, DX_PLAYTYPE_NORMAL, true);
	}
}

void ClearScene::FadeOutUpadte(Input& input)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_), input);
		return;
	}
}

void ClearScene::NormalDraw()
{
	auto size = Application::GetInstance().GetWindowSize();
	DrawRotaGraph(size.width / 2, size.height / 2 * 0.8f, 0.5, 0.0, imgH_, true);

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
}

void ClearScene::FadeDraw()
{
	float rate = static_cast<float>(frame_) /
		static_cast<float>(fade_interval);
	//DrawRotaGraph(320, 240, 1.0f, 0.0f, imgH_, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
