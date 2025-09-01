#include<DxLib.h>
#include "CommandListScene.h"
#include"../Application.h"
#include"SceneController.h"
#include"../Input.h"
namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr int input_list_row_height = 40;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
}

CommandListScene::CommandListScene(SceneController& controller):Scene(controller)
{
}

void CommandListScene::Update(Input& input)
{
	if (input.IsTriggered("ok")) {
		controller_.PopScene();
	}
}

void CommandListScene::Draw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//紫っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 240);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xffaaff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//紫
	DrawBoxAA(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xff00ff, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"コマンド表", 0x008800);
}
