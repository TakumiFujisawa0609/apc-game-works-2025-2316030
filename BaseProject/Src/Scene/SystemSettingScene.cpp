#include<memory>
#include<DxLib.h>
#include "SystemSettingScene.h"
#include"../Application.h"
#include "../Manager/Config.h"
#include"../Manager/SceneController.h"
#include"../Manager/Config.h"
#include "../Manager/InputManager.h"
#include"../Input.h"
namespace {
	constexpr int appear_interval = 20;			//出現までのフレーム
	constexpr int input_list_row_height = 40;	//メニューの１つあたりの高さ
	constexpr int margin_size = 20;				//ポーズメニュー枠の余白
}

auto& ins = InputManager::GetInstance();

SystemSettingScene::SystemSettingScene(SceneController& controller)
	:
	Scene(controller),
	config_(Config::GetInstance())
{
	mouseSensitivity_ = config_.GetMouseSensitivity();
	isVibration_ = config_.isJoyPadVibrationFlag();
	bgmVolume_ = config_.GetBGMVolume();
	seVolume_ = config_.GetSEVolume();
	isFullS_ = config_.IsFullScreen();
	width_ = config_.GetWindowSize().width_;
	height_ = config_.GetWindowSize().height_;

	mainMenuIndex_ = 0;
	prefMenuIndex_ = 0;
	asMenuIndex_ = 0;
	wMenuIndex_ = 0;

	isInitialize_ = false;
	asState_ = AdvancedSettingState::MainMenu;

	// メインメニュー
	menuList_ = {
		L"詳細設定",
		L"入力"
	};
	mainMenuFuncTable_ = {
		{L"詳細設定",[this](Input& input) {
				update_ = &SystemSettingScene::AdvancedSettingUpdate;
				draw_ = &SystemSettingScene::AdvancedSettingDraw;
				frame_ = 0;
			}	
		},
		{L"入力",[this](Input& input) {
				update_ = &SystemSettingScene::PreferencesUpdate;
				draw_ = &SystemSettingScene::PreferencesDraw;
				frame_ = 0;
			}
		}
	};

	// 環境設定
	prefeMenuList_ = {
		L"マウス感度",
		L"パッドの振動",
		L"BGMの音量",
		L"SEの音量",
		L"デフォルトの状態に戻す",
		L"適用",
		L"戻る"
	};
	prefeMenuTable_ = {
		{L"マウス感度",[this](Input&) {
				config_.SetMouseSensitivity(mouseSensitivity_);
			}
		},
		{L"パッドの振動",[this](Input&) {
				config_.SetJoyPadVibration(isVibration_);
			}
		},
		{L"BGMの音量",[this](Input&) {
				config_.SetBGMVolume(bgmVolume_);
			}
		},
		{L"SEの音量",[this](Input&) {
				config_.SetSEVolume(seVolume_);
			}
		},
		{L"デフォルトの状態に戻す",[this](Input&) {

			}
		},
		{L"適用",[this](Input&) {

			}
		},
		{L"戻る",[this](Input& input) {
				controller_.PopScene(input);
			}
		}
	};

	// 詳細設定
	asmList_ = {
		L"表示モード",
		L"ウィンドウサイズ",
		L"戻る"
	};
	asmTable_ = {
		{L"表示モード",[this](Input&) {
				config_.SetFullScreen(isFullS_);
			}
		},
		{L"ウィンドウサイズ",[this](Input&) {
				if (!config_.IsFullScreen()) {
					asState_ = AdvancedSettingState::WindowSize;
				}
			}
		},
		{L"戻る",[this](Input& input) {
				controller_.PopScene(input);
			}
		}
	};

	// ウィンドウメニューの設定
	windowSizeList_ = {
		L"1280×720",
		L"1920×1080"
	};
	windowSizeTable_ = {
		{L"1280×720",[this](Input&) {
				width_ = 1280;
				height_ = 720;
				config_.SetWindowSize(width_, height_);
			}
		},
		{L"1920×1080",[this](Input&) {
				width_ = 1920;
				height_ = 1080;
				config_.SetWindowSize(width_, height_);
			}
		}
	};
}

void SystemSettingScene::Init(Input& input)
{
	isInitialize_ = true;

	// メインメニュー
	InitMenuName(menuList_[mainMenuIndex_], mainMenuFuncTable_, input);

	// 環境設定メニュー
	InitMenuName(prefeMenuList_[prefMenuIndex_], prefeMenuTable_, input);

	// 詳細設定
	InitMenuName(asmList_[asMenuIndex_], asmTable_, input);

	// ウィンドウ切り替えメニュー
	InitMenuName(windowSizeList_[wMenuIndex_], windowSizeTable_, input);
}

void SystemSettingScene::Update(Input& input)
{
	bool mainMenuIndexChanged = false;

	// メインメニューの切り替え
	if(ins.IsTrgDown(KEY_INPUT_E)){
		mainMenuIndex_ = static_cast<int>((mainMenuIndex_ + menuList_.size() - 1) % menuList_.size());
		mainMenuIndexChanged = true;
	}
	else if (ins.IsTrgDown(KEY_INPUT_Q)){
		mainMenuIndex_ = (mainMenuIndex_ + 1) % menuList_.size();
		mainMenuIndexChanged = true;
	}

	// メインメニューのインデックスが変更されたら、対応する関数を呼び出してサブメニューに遷移
	if (mainMenuIndexChanged) {
		// 現在のインデックスに対応するメニュー名を取得
		const std::wstring& selectedMenuName = menuList_[mainMenuIndex_];

		// 関数テーブルから対応する関数を探して実行
		auto it = mainMenuFuncTable_.find(selectedMenuName);
		if (it != mainMenuFuncTable_.end()) {
			it->second(input);
		}
	}

	(this->*update_)(input);
}

void SystemSettingScene::Draw()
{
	const Config::WindowSize& wsize = Config::GetInstance().GetWindowSize();
	//青っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(static_cast<int>(wsize.width_* 0.03125f), static_cast<int>(wsize.height_* 0.04166f),
		static_cast<int>(wsize.width_ - wsize.width_ * 0.03125f), static_cast<int>(wsize.height_ - wsize.height_ * 0.04166f),
		0xaaaaff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//青
	DrawBoxAA(static_cast<float>(wsize.width_ * 0.03125f), static_cast<float>(wsize.height_ * 0.04166f),
		static_cast<float>(wsize.width_ - wsize.width_ * 0.03125f), static_cast<float>(wsize.height_ - wsize.height_ * 0.04166f),
		0x0000ff, false, 3.0f);
	DrawString(static_cast<int>(wsize.width_ * 0.03125f + wsize.width_ * 0.015625f), static_cast<int>(wsize.height_ * 0.04166f + wsize.height_ * 0.020833f), L"システム設定", 0xff0000);

	(this->*draw_)();
}

void SystemSettingScene::DrawUI(void)
{
}

void SystemSettingScene::PreferencesUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		// 現在選択されているメインメニューの項目名を取得
		const std::wstring& selectedMenuName = prefeMenuList_[prefMenuIndex_];

		// 関数テーブルから対応する関数を探して実行
		auto it = prefeMenuTable_.find(selectedMenuName);
		if (it != prefeMenuTable_.end()) {
			it->second(input);
		}
		return;
	}

	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	if (wheelCounter > 1){
		prefMenuIndex_ = static_cast<int>((prefMenuIndex_ + prefeMenuList_.size() - 1) % prefeMenuList_.size());
		wheelCounter = 0;
	}
	else if (wheelCounter <= -1){
		prefMenuIndex_ = (prefMenuIndex_ + 1) % prefeMenuList_.size();
		wheelCounter = 0;
	}

}

void SystemSettingScene::AdvancedSettingUpdate(Input& input)
{
	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	switch (asState_)
	{
	case SystemSettingScene::AdvancedSettingState::MainMenu:
		if (asmList_[asMenuIndex_] == L"表示モード" && ins.IsTrgDown(KEY_INPUT_SPACE)) {
			// フルスクリーン状態を切り替え
			isFullS_ = !isFullS_;

			// 設定をConfigに反映させるために、対応する関数を実行
			auto it = asmTable_.find(L"表示モード");
			if (it != asmTable_.end()) {
				it->second(input);
			}
		}
		if (asmList_[asMenuIndex_] == L"ウィンドウサイズ" && ins.IsTrgDown(KEY_INPUT_SPACE)) {
			// フルスクリーン状態を切り替え
			isFullS_ = !isFullS_;

			// 設定をConfigに反映させるために、対応する関数を実行
			auto it = asmTable_.find(L"ウィンドウサイズ");
			if (it != asmTable_.end()) {
				it->second(input);
			}
		}

		if (input.IsTriggered("ok")) {
			// 現在選択されているメインメニューの項目名を取得
			const std::wstring& selectedMenuName = asmList_[asMenuIndex_];

			// 関数テーブルから対応する関数を探して実行 (サブメニューへの遷移)
			auto it = asmTable_.find(selectedMenuName);
			if (it != asmTable_.end()) {
				it->second(input);
			}
			return;
		}
		if (wheelCounter > 1) {
			asMenuIndex_ = static_cast<int>((asMenuIndex_ + asmList_.size() - 1) % asmList_.size());
			wheelCounter = 0;
		}
		else if (wheelCounter <= -1) {
			asMenuIndex_ = (asMenuIndex_ + 1) % asmList_.size();
			wheelCounter = 0;
		}
		break;
	case SystemSettingScene::AdvancedSettingState::WindowSize:
		if (input.IsTriggered("ok")) {
			const std::wstring& selectedSizeName = windowSizeList_[wMenuIndex_];
			auto it = windowSizeTable_.find(selectedSizeName);
			if (it != windowSizeTable_.end()) {
				// サイズ変更とConfigへの適用を実行
				it->second(input);

				// サイズ決定後、メインメニューに戻る
				asState_ = AdvancedSettingState::MainMenu;
			}
			return;
		}

		if (wheelCounter > 1) {
			wMenuIndex_ = static_cast<int>((wMenuIndex_ + windowSizeList_.size() - 1) % windowSizeList_.size());
			wheelCounter = 0;
		}
		else if (wheelCounter <= -1) {
			wMenuIndex_ = (wMenuIndex_ + 1) % windowSizeList_.size();
			wheelCounter = 0;
		}
		break;
	default:
		break;
	}
	

}

void SystemSettingScene::PreferencesDraw(void)
{
	DrawFormatString(100, 100, GetColor(0, 0, 0), L"pre");
	auto& size = Config::GetInstance().GetWindowSize();
	int line_start_y = static_cast<int>(size.height_ * 0.25f);
	int line_start_x = static_cast<int>(size.width_ * 0.03125f + size.width_ * 0.390625f);
	int lineY = line_start_y;

	auto& currentStr = prefeMenuList_[prefMenuIndex_];
	for (auto& row : prefeMenuList_) {
		int lineX = line_start_x;
		unsigned int col = 0x4444ff;
		if (row == currentStr) {
			DrawString(static_cast<int>(lineX - size.width_ * 0.03125f), lineY, L"⇒", 0xff0000);
			col = 0xff00ff;
			lineX += static_cast<int>(size.width_ * 0.015625f);
		}
		DrawFormatString(static_cast<int>(lineX + size.width_ * 0.0015625f), static_cast<int>(lineY + size.height_ * 0.0020833f), 0x000000, L"%s", row.c_str());
		DrawFormatString(lineX, lineY, col, L"%s", row.c_str());
		lineY += static_cast<int>(size.height_ * 0.0833f);
	}
}

void SystemSettingScene::AdvancedSettingDraw(void)
{
	DrawFormatString(100, 100, GetColor(0, 0, 0), L"advenced");
	auto& size = Config::GetInstance().GetWindowSize();
	int line_start_y = static_cast<int>(size.height_ * 0.25f);
	int line_start_x = static_cast<int>(size.width_ * 0.03125f + size.width_ * 0.390625f);
	int lineY = line_start_y; 

	auto& currentStr = asmList_[asMenuIndex_];
	for (auto& row : asmList_) {
		int lineX = line_start_x;
		unsigned int col = 0x4444ff;
		if (row == currentStr) {
			DrawString(static_cast<int>(lineX - size.width_ * 0.03125f), lineY, L"⇒", 0xff0000);
			col = 0xff00ff;
			lineX += static_cast<int>(size.width_ * 0.015625f);
		}
		std::wstring display_text = L"";

		if (row == L"表示モード") {
			// 現在の設定モードに応じた文字列を取得
			const std::wstring mode_text = isFullS_ ? L" (フルスクリーン)" : L" (ウィンドウモード)";
			display_text = mode_text;
			if (row == currentStr) {
				DrawFormatString(static_cast<int>(lineX + size.width_ * 0.55f), static_cast<int>(lineY + size.height_ * 0.0020833f), 0x00ff00, L"[SPACE]で切り替え");
			}
		}
		else if (row == L"ウィンドウサイズ") {
			// 現在のウィンドウサイズを表示
			display_text = L" (" + std::to_wstring(width_) + L"×" + std::to_wstring(height_) + L")";
			if (row == currentStr && !isFullS_ && asState_ == AdvancedSettingState::MainMenu) {
				DrawFormatString(static_cast<int>(lineX + size.width_ * 0.55f), static_cast<int>(lineY + size.height_ * 0.0020833f), 0x00ff00, L"[OK]で選択");
			}
			else if (isFullS_) {
				// フルスクリーンの場合は選択不可であることを示す
				// 描画位置を row の高さに合わせて lineY を使用
				DrawFormatString(static_cast<int>(lineX + size.width_ * 0.55f), static_cast<int>(lineY + size.height_ * 0.0020833f), 0x888888, L" (フルスクリーン時は変更不可)");
			}
		}

		DrawFormatString(static_cast<int>(lineX + size.width_ * 0.0015625f), static_cast<int>(lineY + size.height_ * 0.0020833f), 0x000000, L"%s", row.c_str());
		DrawFormatString(lineX, lineY, col, L"%s", row.c_str());

		DrawFormatString(static_cast<int>(lineX + size.width_ * 0.3125f), static_cast<int>(lineY + size.height_ * 0.0020833f), 0x000000, L"%s", display_text.c_str());

		// 次の行へ移動
		lineY += static_cast<int>(size.height_ * 0.0833f);
	}

	if (asState_ == AdvancedSettingState::WindowSize) {
		int wLineY = static_cast<int>(lineY + size.height_ * 0.02f);			// lineYは既に最後の項目の次の位置になっているため、少しだけマージンを加える
		int wLineX = static_cast<int>(line_start_x + size.width_ * 0.05f);		// メインメニューから少し右にずらす

		auto& currentWStr = windowSizeList_[wMenuIndex_];

		for (auto& wRow : windowSizeList_) {
			unsigned int col = 0xeeeeee;
			int currentWLineX = wLineX;

			if (wRow == currentWStr) {
				DrawString(static_cast<int>(currentWLineX - size.width_ * 0.03125f), wLineY, L"▶", 0xffff00);
				col = 0xffffff;
				currentWLineX += static_cast<int>(size.width_ * 0.015625f);
			}

			DrawFormatString(static_cast<int>(currentWLineX + size.width_ * 0.0015625f), static_cast<int>(wLineY + size.height_ * 0.0020833f), 0x000000, L"%s", wRow.c_str());
			DrawFormatString(currentWLineX, wLineY, col, L"%s", wRow.c_str());
			wLineY += static_cast<int>(size.height_ * 0.0833f);
		}
	}
}

void SystemSettingScene::InitMenuName(std::wstring menuListIndex, std::map<std::wstring, std::function<void(Input&)>> table, Input& input)
{
	const std::wstring& initialMenuName = menuListIndex;
	auto it = table.find(initialMenuName);

	if (it != table.end()) {
		it->second(input);
	}
}
