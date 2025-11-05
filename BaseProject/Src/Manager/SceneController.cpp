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

	//// 深度バッファ用スクリーン
	//// ( １チャンネル浮動小数点２４ビットテクスチャ )
	//SetCreateDrawValidGraphChannelNum(1);
	//SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	//SetCreateGraphChannelBitDepth(24);

	//auto sizeW = Application::GetInstance().GetWindowSize();

	//// 新しいシーン用のスクリーンの作成
	//newScreenH_ = MakeScreen(sizeW.width, sizeW.height, true);

	//SetCreateDrawValidGraphChannelNum(0);
	//SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	//SetCreateGraphChannelBitDepth(0);

	scenes_.back()->Init(input);//シーンの初期化

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

void SceneController::DrawUI()
{
	for (auto& scene : scenes_) {
		scene->DrawUI();
	}
}

void SceneController::PushScene(std::shared_ptr<Scene> scene, Input& input)
{
	auto size = Application::GetInstance().GetWindowSize();

	//// 新しい裏画面を作成
	//int newScreenH = MakeScreen(size.width, size.height, true);

	//assert(newScreenH == -1);

	//// 作成した画面ハンドルをシーンに設定
	//scene->SetRenderTarget(newScreenH);

	// シーンを積む
	scenes_.push_back(scene);

	//// 描画先を裏画面に設定
	//SetDrawScreen(newScreenH);

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
	for (auto& s : scenes_)
	{
		DeleteGraph(s->GetRenderTarget());
	}
	scenes_.clear();
	scenes_.push_back(scene);

	auto sizeW = Application::GetInstance().GetWindowSize();
	int newScreenH = MakeScreen(sizeW.width, sizeW.height);
	scenes_.back()->SetRenderTarget(newScreenH);

	scenes_.back()->Init(input);
	Application::GetInstance().ResetDeltaTime();
}

int SceneController::GetDepthScreen(void) const
{
	return newScreenH_;
}


