#include<DxLib.h>
#include "CommandListScene.h"
#include"../Application.h"
#include "../Manager/Config.h"
#include"../Manager/SceneController.h"
#include"../Input.h"
namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr int input_list_row_height = 40;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
}

CommandListScene::CommandListScene(SceneController& controller):Scene(controller)
{
}

void CommandListScene::Init(Input& input)
{
}

void CommandListScene::Update(Input& input)
{
	if (input.IsTriggered("ok")) {
		controller_.PopScene(input);
	}
}

void CommandListScene::Draw()
{
	const Config::WindowSize& wsize = Config::GetInstance().GetWindowSize();
	//紫っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(wsize.width_ * 0.03125f, wsize.height_ * 0.04166f,
		wsize.width_ - wsize.width_ * 0.03125f, wsize.height_ - wsize.height_ * 0.04166f,
		0xffaaff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//紫
	DrawBoxAA(static_cast<float>(wsize.width_ * 0.03125f), static_cast<float>(wsize.height_ * 0.04166f),
		static_cast<float>(wsize.width_ - wsize.width_ * 0.03125f), static_cast<float>(wsize.height_ - wsize.height_ * 0.04166f),
		0xff00ff, false, 3.0f);
	DrawString(wsize.width_ * 0.03125f + wsize.width_ * 0.015625f, wsize.height_ * 0.04166f + wsize.height_ * 0.020833f, L"コマンド表", 0x008800);
}

void CommandListScene::DrawUI(void)
{
}
