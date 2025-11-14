#include<memory>
#include<DxLib.h>
#include "SystemSettingScene.h"
#include"../Application.h"
#include"../Manager/SceneController.h"
#include"../Manager/Config.h"
#include "../Manager/InputManager.h"
#include"../Input.h"
namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr int input_list_row_height = 40;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
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
		L"BGMの音量"
		L"SEの音量"
		L"デフォルトの状態に戻す"
		L"適用"
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
		}
	};

	// 詳細設定
	asmList_ = {
		L"表示モード",
		L"ウィンドウサイズ",
	};

	asmTable_ = {
		{L"表示モード",[this](Input&) {
				config_.SetFullScreen(isFullS_);
			}
		},
		{L"ウィンドウサイズ",[this](Input&) {
				if (!config_.IsFullScreen()) {
					config_.SetWindowSize(width_,height_);
				}
			}
		}
	};
}

void SystemSettingScene::Init(Input& input)
{	
	// メインメニュー
	InitMenuName(menuList_[mainMenuIndex_], mainMenuFuncTable_, input);

	// 環境設定メニュー
	InitMenuName(prefeMenuList_[prefMenuIndex_], prefeMenuTable_, input);

	// 詳細設定
	InitMenuName(asmList_[asMenuIndex_], asmTable_, input);

}

void SystemSettingScene::Update(Input& input)
{
	if (input.IsTriggered("ok")) {
		controller_.PopScene(input);
		return;
	}

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
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//青っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xaaaaff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//青
	DrawBoxAA(static_cast<float>(margin_size), static_cast<float>(margin_size),
		static_cast<float>(wsize.width - margin_size), static_cast<float>(wsize.height - margin_size),
		0x0000ff, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"システム設定", 0xff0000);

	(this->*draw_)();
}

void SystemSettingScene::DrawUI(void)
{
}

void SystemSettingScene::PreferencesUpdate(Input&)
{
	// 操作系の設定
	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	if (wheelCounter > 1){
		prefMenuIndex_ = static_cast<int>((prefMenuIndex_ + menuList_.size() - 1) % menuList_.size());
	}
	else if (wheelCounter <= -1){
		prefMenuIndex_ = (prefMenuIndex_ + 1) % prefeMenuList_.size();
	}

}

void SystemSettingScene::AdvancedSettingUpdate(Input&)
{
	// スクリーン系の設定
	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	if (wheelCounter > 1){
		asMenuIndex_ = static_cast<int>((asMenuIndex_ + menuList_.size() - 1) % menuList_.size());
	}
	else if (wheelCounter <= -1){
		asMenuIndex_ = (asMenuIndex_ + 1) % prefeMenuList_.size();
	}

}

void SystemSettingScene::PreferencesDraw(void)
{
	DrawFormatString(100, 100, GetColor(0, 0, 0), L"pre");
}

void SystemSettingScene::AdvancedSettingDraw(void)
{
	DrawFormatString(100, 100, GetColor(0, 0, 0), L"advenced");
}

void SystemSettingScene::InitMenuName(std::wstring menuListIndex, std::map<std::wstring, std::function<void(Input&)>> table, Input& input)
{
	const std::wstring& initialMenuName = menuListIndex;
	auto it = table.find(initialMenuName);

	if (it != table.end()) {
		it->second(input);
	}
}
