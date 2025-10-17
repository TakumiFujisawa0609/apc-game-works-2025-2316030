#include "DxLib.h"
#include <cassert>
#include "SceneController.h"
#include "../Application.h"
#include"../Scene/Scene.h"

void SceneController::ChangeScene(std::shared_ptr<Scene> scene, Input& input)
{
	for (auto& s : scenes_)
	{
		DeleteGraph(s->GetRenderTarget());
	}
	scenes_.clear();

	if (scenes_.empty()) 
	{
		scenes_.push_back(scene);
	}
	else 
	{
		scenes_.back() = scene;
	}

	auto sizeW = Application::GetInstance().GetWindowSize();

	// 新しいシーン用のスクリーンの作成
	int newScreenH = MakeScreen(sizeW.width, sizeW.height, true);

	//assert(newScreenH == -1);

	// 作成した画面ハンドルをシーンに設定
	scene->SetRenderTarget(newScreenH);

	// 初期化時の描画先を新しいハンドルに設定
	SetDrawScreen(newScreenH);

	scenes_.back()->Init(input);//シーンの初期化
	
	// メインの裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	Application::GetInstance().ResetDeltaTime();
}

void SceneController::Update(Input& input)
{
	scenes_.back()->Update(input);
}

void SceneController::Draw()
{
	for (auto& scene : scenes_) {
		scene->Draw();
	}
}

void SceneController::PushScene(std::shared_ptr<Scene> scene, Input& input)
{
	auto size = Application::GetInstance().GetWindowSize();

	// 新しい裏画面を作成
	int newScreenH = MakeScreen(size.width, size.height, true);

	//assert(newScreenH == -1);

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

void SceneController::JumpScene(std::shared_ptr<Scene> scene)
{
	scenes_.clear();
	scenes_.push_back(scene);
}


