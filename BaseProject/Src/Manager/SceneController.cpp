#include "DxLib.h"
#include <cassert>
#include "SceneController.h"
#include "../Manager/Config.h"
#include "../Application.h"
#include "../Scene/Scene.h"
#include "../Scene/PauseScene.h"
#include "../Scene/SystemSettingScene.h"
#include "../Scene/UnlockScene.h"
#include "../Scene/GameScene.h"

void SceneController::ChangeScene(std::shared_ptr<Scene> scene, Input& input)
{
	for (auto& s : scenes_){
		if (s->GetRenderTarget() != -1) {
			DeleteGraph(s->GetRenderTarget());
		}
	}
	scenes_.clear();

	if (scenes_.empty()) {
		scenes_.push_back(scene);
	}
	else {
		scenes_.back() = scene;
	}

	// 深度バッファ用スクリーン
	// ( １チャンネル浮動小数点２４ビットテクスチャ )
	SetCreateDrawValidGraphChannelNum(1);
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateGraphChannelBitDepth(24);

	auto& sizeW = Config::GetInstance().GetWindowSize();

	// メイン用スクリーンの作成
	mainScreen_ = MakeScreen(sizeW.width_, sizeW.height_, true);

	// 深度バッファ用のスクリーンの作成
	depthScreen_ = MakeScreen(sizeW.width_, sizeW.height_, true);

	SetCreateDrawValidGraphChannelNum(0);
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateGraphChannelBitDepth(0);

	blur1Screen_ = MakeScreen(sizeW.width_, sizeW.height_, true);
	blur2Screen_ = MakeScreen(sizeW.width_, sizeW.height_, true);

	scenes_.back()->Init(input);//シーンの初期化

	Application::GetInstance().ResetDeltaTime();
}

void SceneController::Update(Input& input)
{
	scenes_.back()->Update(input);
}

void SceneController::Draw(void)
{
	//SetDrawScreen(mainScreen_);
	ClearDrawScreen();
	for (auto& scene : scenes_) {
		if (std::dynamic_pointer_cast<UnlockScene>(scene) != nullptr) {
			continue;
		}
		if (std::dynamic_pointer_cast<PauseScene>(scene) != nullptr) {
			continue;
		}
		if (std::dynamic_pointer_cast<SystemSettingScene>(scene) != nullptr) {
			continue;
		}
		scene->Draw();
	}


	SetDrawScreen(DX_SCREEN_BACK);
	//ClearDrawScreen();

	//if (auto gameScene = std::dynamic_pointer_cast<GameScene>(scenes_.back())) {
	//	//// a. ブラーテクスチャの更新（mainScreen_の内容を使って）
	//	//const auto& sizeW = Config::GetInstance().GetWindowSize();
	//	//int mainScreen = GetMainScreen();
	//	//int blur1Screen = GetBlur1Screen();
	//	//int blur2Screen = GetBlur2Screen();

	//	//// GameScene::DrawMainGameから移動
	//	//GraphFilterRectBlt(mainScreen, blur1Screen, 0, 0, sizeW.width_, sizeW.height_, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200);
	//	//GraphFilterRectBlt(blur1Screen, blur2Screen, 0, 0, sizeW.width_, sizeW.height_, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200);

	//	//// b. DoF ポストエフェクトの描画（DX_SCREEN_BACKに出力される）
	//	//gameScene->DrawPostEffect();

	//	DrawGraph(0, 0, GetMainScreen(), false);

	//}
	//else {
	//	DrawGraph(0, 0, GetMainScreen(), false);
	//}
}

void SceneController::DrawUI(void)
{
	for (auto& scene : scenes_) {
		scene->DrawUI();
	}
}

void SceneController::PushScene(std::shared_ptr<Scene> scene, Input& input)
{
	auto& size = Config::GetInstance().GetWindowSize();

	// 新しい裏画面を作成
	int newScreenH = MakeScreen(size.width_, size.height_, true);

	assert(newScreenH != -1);

	// 作成した画面ハンドルをシーンに設定
	scene->SetRenderTarget(newScreenH);

	// シーンを積む
	scenes_.push_back(scene);

	// 描画先を裏画面に設定
	SetDrawScreen(newScreenH);

	// 追加されたシーンの初期化
	scenes_.back()->Init(input);

	// 描画先をデフォルトに戻す
	SetDrawScreen(DX_SCREEN_BACK);
}

void SceneController::PopScene(Input& input)
{
	if (scenes_.size() > 1) {
		int screenH = scenes_.back()->GetRenderTarget();
		DeleteGraph(screenH);
		scenes_.pop_back();
	}
}

void SceneController::JumpScene(std::shared_ptr<Scene> scene, Input& input)
{
	for (auto& s : scenes_){
		DeleteGraph(s->GetRenderTarget());
	}
	scenes_.clear();
	scenes_.push_back(scene);

	const auto& sizeW = Config::GetInstance().GetWindowSize();
	int newScreenH = MakeScreen(sizeW.width_, sizeW.height_);
	scenes_.back()->SetRenderTarget(newScreenH);

	scenes_.back()->Init(input);
	Application::GetInstance().ResetDeltaTime();
}

void SceneController::DrawPushScene(void)
{
	for (const auto& scene : scenes_) {
		auto unlockScene = std::dynamic_pointer_cast<UnlockScene>(scene);
		if (unlockScene != nullptr) {
			unlockScene->Draw();
		}
		auto pauseScene = std::dynamic_pointer_cast<PauseScene>(scene);
		if (pauseScene != nullptr) {
			pauseScene->Draw();
		}
		auto settingScene = std::dynamic_pointer_cast<SystemSettingScene>(scene);
		if (settingScene != nullptr) {
			settingScene->Draw();
		}
	}
}

int SceneController::GetMainScreen(void) const
{
	return mainScreen_;
}

int SceneController::GetDepthScreen(void) const
{
	return depthScreen_;
}

int SceneController::GetBlur1Screen(void) const
{
	return blur1Screen_;
}

int SceneController::GetBlur2Screen(void) const
{
	return blur2Screen_;
}


