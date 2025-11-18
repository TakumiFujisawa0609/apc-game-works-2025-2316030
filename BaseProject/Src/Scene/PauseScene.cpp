#include<DxLib.h>
#include "PauseScene.h"
#include "../Manager/Config.h"
#include"../Manager/InputManager.h"
#include"../Manager/SceneController.h"
#include"../Application.h"
#include"../Manager/Camera.h"
#include"../Input.h"
#include"CommandListScene.h"
#include"KeyConfigScene.h"
#include"SystemSettingScene.h"
#include"TitleScene.h"

using namespace std;

namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr int input_list_row_height = 40;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
}

PauseScene::PauseScene(SceneController& controller) :
	Scene(controller),
	frame_(0),
	update_(&PauseScene::AppearUpdate),
	draw_(&PauseScene::ProcessDraw)
{
	menuList_ = {
					L"コマンド表",
					L"設定メニュー",
					L"キーコンフィグ",
					L"戻る",
					L"タイトルに戻る",
					L"ゲームを終了する"
	};
	menuFuncTable_ = {
		{L"コマンド表",[this](Input& input) {
				controller_.PushScene(make_shared<CommandListScene>(controller_),input);
			}
		},
		{L"設定メニュー",[this](Input& input) {
				controller_.PushScene(make_shared<SystemSettingScene>(controller_),input);
				}},
		{L"キーコンフィグ",[this](Input& input) {
				controller_.PushScene(make_shared<KeyConfigScene>(controller_,input),input);
			}},
		{L"戻る",[this](Input&) {
				update_ = &PauseScene::DisappearUpdate;
				draw_ = &PauseScene::ProcessDraw;
				Application::GetInstance().GetCamera()->SetOperableCamera(true);
			}
		},
		{L"タイトルに戻る",[this](Input& input) {
				controller_.JumpScene(make_shared<TitleScene>(controller_),input);
				return;
			}
		},
		{L"ゲームを終了する",[this](Input&) {
				Application::GetInstance().SetGemeEnd(true);
			}
		}
	};
}

void PauseScene::Init(Input& input)
{
}

void PauseScene::Update(Input& input)
{
	(this->*update_)(input);
}

void PauseScene::Draw()
{
	(this->*draw_)();
}

void PauseScene::DrawUI(void)
{
}

void PauseScene::AppearUpdate(Input& input)
{
	if (++frame_ >= appear_interval) {
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		return;
	}
}

void PauseScene::DisappearUpdate(Input& input)
{
	if (--frame_ <= 0) {
		controller_.PopScene(input);
		return;
	}
}

void PauseScene::NormalUpdate(Input& input)
{
	auto& ins = InputManager::GetInstance();
	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	if (input.IsTriggered("pause")) {
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::ProcessDraw;
		return;
	}
	if (input.IsTriggered("up")|| wheelCounter > 1) {
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

void PauseScene::ProcessDraw()
{
	const Config::WindowSize& wsize = Config::GetInstance().GetWindowSize();
	int centerY = wsize.height_ / 2;//画面中心Y
	int frameHalfHeight = static_cast<int>(wsize.height_ - wsize.height_ * 0.4166f * 2) / 2;//枠の高さの半分

	//出現・消滅時の高さ変化率(0.0～1.0)
	float rate = static_cast<float>(frame_) / 
					static_cast<float>(appear_interval);

	frameHalfHeight *= static_cast<int>(rate);

	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(static_cast<int>(wsize.width_ * 0.03125f), static_cast<int>(centerY - frameHalfHeight),
		static_cast<int>(wsize.width_ - wsize.width_ * 0.03125f), static_cast<int>(centerY + frameHalfHeight),
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(static_cast<float>(wsize.width_ * 0.03125f), static_cast<float>(centerY - frameHalfHeight),
		static_cast<float>(wsize.width_ - wsize.width_*0.03125f), static_cast<float>(centerY + frameHalfHeight),
		0xfffffff, false, 3.0f);
}

void PauseScene::NormalDraw()
{
	
	const Config::WindowSize& wsize = Config::GetInstance().GetWindowSize();
	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(static_cast<int>(wsize.width_ * 0.03125f), static_cast<int>(wsize.height_ * 0.04166f),
		static_cast<int>(wsize.width_ - wsize.width_ * 0.03125f), static_cast<int>(wsize.height_ - wsize.height_ * 0.04166f),
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(static_cast<float>(wsize.width_ * 0.03125f), static_cast<float>(wsize.height_ * 0.04166f),
		static_cast<float>(wsize.width_ - wsize.width_ * 0.03125f), static_cast<float>(wsize.height_ - wsize.height_ * 0.04166f),
		0xfffffff, false, 3.0f);
	DrawString(static_cast<int>(wsize.width_ * 0.03125f + 10), static_cast<int>(wsize.height_ * 0.04166f + wsize.height_ * 0.020833f), L"Pause Scene", 0x0000ff);
	DrawMenuList();
}

void PauseScene::DrawMenuList()
{
	auto& wsize = Config::GetInstance().GetWindowSize();
	int line_start_y = static_cast<int>(wsize.height_ * 0.04166f + wsize.height_ * 0.3125f);
	int line_start_x = static_cast<int>(wsize.width_ * 0.03125f + wsize.width_* 0.390625f);
	int lineY = line_start_y;
	
	auto currentStr = menuList_[currentIndex_];
	for (auto& row : menuList_) {
		int lineX = line_start_x;
		unsigned int col = 0x4444ff;
		if (row == currentStr) {
			DrawString(static_cast<int>(lineX - wsize.width_ * 0.03125f), lineY, L"⇒",0xff0000);
			col = 0xff00ff;
			lineX += static_cast<int>(wsize.width_ * 0.015625f);
		}
		DrawFormatString(static_cast<int>(lineX+wsize.width_* 0.0015625f), static_cast<int>(lineY+wsize.height_* 0.0020833f), 0x000000, L"%s", row.c_str());
		DrawFormatString(lineX, lineY, col, L"%s", row.c_str());
		lineY += input_list_row_height;
	}
	
}
