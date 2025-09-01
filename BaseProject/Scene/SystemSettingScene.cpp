#include<DxLib.h>
#include "SystemSettingScene.h"
#include"../Application.h"
#include"SceneController.h"
#include"../Input.h"
namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr int input_list_row_height = 40;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
}

SystemSettingScene::SystemSettingScene(SceneController& controller):Scene(controller)
{
}

void SystemSettingScene::Update(Input& input)
{
	if (input.IsTriggered("ok")) {
		controller_.PopScene();
		return;
	}
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
	DrawBoxAA(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0x0000ff, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"システム設定", 0xff0000);
}
