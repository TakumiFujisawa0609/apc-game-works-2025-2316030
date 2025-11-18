#include "TitleScene.h"
#include<DxLib.h>
#include<memory>
#include<cassert>
#include "../Application.h"
#include "../Manager/Config.h"
#include "../Manager/InputManager.h"
#include"../Manager/SceneController.h"
#include"../Manager/ResourceManager.h"
#include"SystemSettingScene.h"
#include"GameScene.h"
#include"../Input.h"

namespace {
	constexpr int fade_interval = 30;
}

namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr float input_list_row_height = 1.0f / 12.0f;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
}

TitleScene::TitleScene(SceneController& controller)
	:
	Scene(controller),
	titleH_(-1)
{
	soundH_ = resMng_.Load(ResourceManager::SRC::DECIDE_SE).handleId_;
	ChangeVolumeSoundMem(255, soundH_);

	menuList_ = {
		L"continue",
		L"config",
		L"quit game"
	};
	menuFuncTable_ = {
		{L"continue",[this](Input& input) {
				update_ = &TitleScene::FadeOutUpdate;
				draw_ = &TitleScene::FadeDraw;
				frame_ = 0;
			}
		},
		{L"config",[this](Input& input) {
				controller_.PushScene(std::make_shared<SystemSettingScene>(controller_),input);
				}},
		{L"quit game",[this](Input& input) {
				Application::GetInstance().SetGemeEnd(true);
			}}
	};
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

void TitleScene::DrawUI(void)
{
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
	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	if (input.IsTriggered("up") || wheelCounter > 1) {
		currentIndex_ = static_cast<int>((currentIndex_ + menuList_.size() - 1) % menuList_.size());
		wheelCounter = 0;
	}
	else if (input.IsTriggered("down") || wheelDelta <= -1) {
		currentIndex_ = (currentIndex_ + 1) % menuList_.size();
		wheelCounter = 0;
	}
	if (input.IsTriggered("ok")) {
		auto selectedName = menuList_[currentIndex_];
		menuFuncTable_[selectedName](input);
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

	auto& size = Config::GetInstance().GetWindowSize();
	DrawRotaGraph(size.width_ / 2, static_cast<int>(size.height_ / 2 * 0.8f), 0.4, 0.0, titleH_, true);

	//const TCHAR* text_to_display = _T("Space or Aボタン");
	//int text_width = GetDrawStringWidth(text_to_display, static_cast<int>(_tcslen(text_to_display)));

	//// X座標: 画面中央 (画面幅 / 2) からテキスト幅の半分を引く
	//int draw_x = (size.width / 2) - (text_width / 2);

	//// Y座標: 画面全体の高さの 4分の3 の位置
	//int draw_y = (size.height * 3) / 4;

	//// 3. テキストを描画

	//// 赤色で描画
	//int color = GetColor(255, 255, 255); // 白にする場合は GetColor(255, 255, 255)

	//// 描画関数でテキストを表示
	//DrawString(draw_x, draw_y, text_to_display, color);
	//DrawString(10, 10, L"Title Scene", 0xffffff);

	DrawMenuList();
}

void TitleScene::DrawMenuList(void)
{
	auto& size = Config::GetInstance().GetWindowSize();
	int line_start_y = static_cast<int>(size.height_ * 0.5625f);
	int line_start_x = static_cast<int>(size.width_ * 0.421875f);
	int lineY = line_start_y;
	auto& currentStr = menuList_[currentIndex_];
	for (auto& row : menuList_) {
		int lineX = line_start_x;
		unsigned int col = 0x4444ff;

		// 選択カーソル (⇒) の描画
		if (row == currentStr) {
			lineX += static_cast<int>(size.width_ * 0.01625f);
			DrawString(lineX, lineY, L"⇒", 0xff0000);
			col = 0xff00ff;
			lineX += static_cast<int>(size.width_ * 0.015625f);
		}

		// --- 項目テキスト描画 (影/アウトライン) ---
		int shadow_offset_x = static_cast<int>(size.width_ * 0.0015625f);
		int shadow_offset_y = static_cast<int>(size.height_ * 0.0020833f);

		DrawFormatString(lineX + shadow_offset_x, lineY + shadow_offset_y, 0x000000, L"%s", row.c_str());

		// --- 項目テキスト描画 (本体) ---
		DrawFormatString(lineX, lineY, col, L"%s", row.c_str());
		lineY += static_cast<int>(size.height_ * input_list_row_height);
	}
}
