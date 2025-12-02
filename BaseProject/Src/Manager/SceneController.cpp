#include "DxLib.h"
#include <cassert>
#include "SceneController.h"
#include "../Manager/Config.h"
#include "../Application.h"
#include "../Scene/Scene.h"
#include "../Scene/PauseScene.h"
#include "../Scene/SystemSettingScene.h"
#include "../Scene/UnlockScene.h"

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

	const auto& sizeW = Config::GetInstance().GetWindowSize();

	// 新しいシーン用のスクリーンの作成
	mainScreen_ = MakeScreen(sizeW.width_, sizeW.height_, true);

	// 深度バッファ用スクリーン
	// ( １チャンネル浮動小数点２４ビットテクスチャ )
	SetCreateDrawValidGraphChannelNum(1);
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateGraphChannelBitDepth(24);

	depthScreen_ = MakeScreen(sizeW.width_, sizeW.height_, false);

	SetCreateDrawValidGraphChannelNum(0);
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateGraphChannelBitDepth(0);

	//シーンの初期化
	scenes_.back()->Init(input);

	Application::GetInstance().ResetDeltaTime();
}

void SceneController::Update(Input& input)
{
	scenes_.back()->Update(input);
}

void SceneController::Draw(void)
{
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
}

void SceneController::DrawUI(void)
{
	for (auto& scene : scenes_) {
		scene->DrawUI();
	}
}

void SceneController::PushScene(std::shared_ptr<Scene> scene, Input& input)
{
	const auto& size = Config::GetInstance().GetWindowSize();

	// 新しい裏画面を作成
	mainScreen_=MakeScreen(size.width_, size.height_, true);

	// 作成した画面ハンドルをシーンに設定
	scene->SetRenderTarget(mainScreen_);

	// シーンを積む
	scenes_.push_back(scene);

	// 描画先を裏画面に設定
	SetDrawScreen(mainScreen_);

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

int SceneController::GetDepthScreen(void) const
{
	return depthScreen_;
}