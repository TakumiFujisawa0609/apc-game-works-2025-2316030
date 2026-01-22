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
	constexpr int appear_interval = 20;						//出現までのフレーム
	constexpr float input_list_row_height = 1.0f / 12.0f;	//メニューの１つあたりの高さ
	constexpr int margin_size = 20;							//ポーズメニュー枠の余白
}

TitleScene::TitleScene(SceneController& controller)
	:
	Scene(controller),
	titleH_(-1),
	prevPOVDirection_(-1)
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

	DINPUT_JOYSTATE Input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &Input);

	// 現在のPOVの値
	int currentPOV = Input.POV[0];
	// 現在のボタンが押下されたか
	bool currentAPressed = (Input.Buttons[0] != 0);
	// スティックのY軸入力
	float stickY = Input.Y / STICK_SCALE;
	// スティックのX軸入力
	float stickX = Input.X / STICK_SCALE;

	// スティックの上を押下されたか
	static bool stickUpPrev = false;
	// スティックの下を押下されたか
	static bool stickDownPrev = false;
	// スティックの左を押下されたか
	static bool stickLeftPrev = false;
	// スティックの右を押下されたか
	static bool stickRightPrev = false;

	// スティック上
	bool stickUp = stickY < -STICK_THRESHOLD;
	// スティック下
	bool stickDown = stickY > STICK_THRESHOLD;
	// スティック左
	bool stickLeft = stickX < -STICK_THRESHOLD;
	// スティック右
	bool stickRight = stickX > STICK_THRESHOLD;

	// スティックの上を押下されたか
	bool stickUpPressed = (stickUp && !stickUpPrev);
	// スティックの下を押下されたか
	bool stickDownPressed = (stickDown && !stickDownPrev);
	// スティックの左を押下されたか
	bool stickLeftPressed = (stickLeft && !stickLeftPrev);
	// スティックの右を押下されたか
	bool stickRightPressed = (stickRight && !stickRightPrev);

	// 上方向の入力
	bool upPressed = (currentPOV == POV_UP && prevPOVDirection_ != 0) || stickUpPressed;
	// 下方向の入力
	bool downPressed = (currentPOV == POV_DOWN && prevPOVDirection_ != POV_DOWN) || stickDownPressed;
	// 左方向の入力
	bool leftPressed = (currentPOV == POV_LEFT && prevPOVDirection_ != POV_LEFT) || stickLeftPressed;
	// 右方向の入力
	bool rightPressed = (currentPOV == POV_RIGHT && prevPOVDirection_ != POV_RIGHT) || stickRightPressed;
	// 決定ボタンの入力
	bool aPressed = ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);

	// 上方向を入力された場合
	if (input.IsTriggered("up")|| upPressed || wheelCounter > 1) {
		currentIndex_ = static_cast<int>((currentIndex_ + menuList_.size() - 1) % menuList_.size());
		wheelCounter = 0;
	}
	else if (input.IsTriggered("down")|| upPressed || wheelDelta <= -1) {
		currentIndex_ = (currentIndex_ + 1) % menuList_.size();
		wheelCounter = 0;
	}
	// 決定ボタンの入力
	if (input.IsTriggered("ok")|| aPressed) {
		PlaySoundMem(soundH_, DX_PLAYTYPE_BACK, true);
		auto selectedName = menuList_[currentIndex_];
		menuFuncTable_[selectedName](input);
		return;
	}

EndInputUpdate:

	// 各入力の初期化
	prevPOVDirection_ = currentPOV;
	stickUpPrev = stickUp;
	stickDownPrev = stickDown;
	stickLeftPrev = stickLeft;
	stickRightPrev = stickRight;

	// 選択時の点滅カウンターの45フレームごとに計算する
	blinkCounter_ = (blinkCounter_ + 1) % 45;
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
	// タイトル画像の描画
	DrawRotaGraph(size.width_ / 2, static_cast<int>(size.height_ / 2 * 0.8f), 0.4, 0.0, titleH_, true);

	// アクションボタンの表示
	DrawMenuList();
}

void TitleScene::DrawMenuList(void)
{
	auto& size = Config::GetInstance().GetWindowSize();
	int line_start_y = static_cast<int>(size.height_ * 0.5625f);
	int line_start_x = static_cast<int>(size.width_ * 0.421875f);
	int lineY = line_start_y;
	auto& currentStr = menuList_[currentIndex_];

	// 点滅表示判定 (例: 30フレーム周期のうち、前半15フレームは表示、後半15フレームは非表示)
	bool isVisible = (blinkCounter_ < 20);

	for (auto& row : menuList_) {
		int lineX = line_start_x;
		unsigned int col = 0x4444ff;

		// 選択中の行かどうか
		bool isSelected = (row == currentStr);

		// 選択中の行で、かつ非表示タイミングであれば、この行の描画処理をすべてスキップ
		if (isSelected && !isVisible) {
			lineY += static_cast<int>(size.height_ * input_list_row_height);
			continue;
		}

		// 選択時の文字の色の変更
		if (isSelected) {
			col = 0xffffff;
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
