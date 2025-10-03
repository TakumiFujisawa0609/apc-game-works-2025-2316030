#include <DxLib.h>
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneController.h"
#include"../Manager/InputManager.h"
#include"../Manager/Camera.h"
#include"../Application.h"
#include"../Input.h"
#include"GameScene.h"
#include "UnlickScene.h"

using namespace std;

namespace {
	constexpr int appear_interval = 20;//出現までのフレーム
	constexpr int input_list_row_height = 40;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
}

UnlickScene::UnlickScene(SceneController& controller)
	:
	Scene(controller),
	frame_(0),
	update_(&UnlickScene::AppearUpdate),
	draw_(&UnlickScene::ProcessDraw)
{
	Application::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT, AsoUtility::VECTOR_ZERO,false);
}

UnlickScene::~UnlickScene(void)
{
}

void UnlickScene::Init(Input& input)
{
}

void UnlickScene::Update(Input& input)
{
	(this->*update_)(input);
}

void UnlickScene::Draw(void)
{
	(this->*draw_)();
}

void UnlickScene::AppearUpdate(Input& input)
{
	if (++frame_ >= appear_interval) {
		update_ = &UnlickScene::NormalUpdate;
		draw_ = &UnlickScene::NormalDraw;
		return;
	}
}

void UnlickScene::NormalUpdate(Input& input)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_SPACE)) {
		update_ = &UnlickScene::DisappearUpdate;
		draw_ = &UnlickScene::ProcessDraw;
		return;
	}
}

void UnlickScene::DisappearUpdate(Input& input)
{
	if (--frame_ <= 0) {
		controller_.PopScene();
		return;
	}
}

void UnlickScene::ProcessDraw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	int centerY = wsize.height / 2;//画面中心Y
	int frameHalfHeight = (wsize.height - margin_size * 2) / 2;//枠の高さの半分

	//出現・消滅時の高さ変化率(0.0～1.0)
	float rate = static_cast<float>(frame_) /
		static_cast<float>(appear_interval);

	frameHalfHeight *= rate;

	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(margin_size, centerY - frameHalfHeight,
		wsize.width - margin_size, centerY + frameHalfHeight,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(margin_size, centerY - frameHalfHeight,
		wsize.width - margin_size, centerY + frameHalfHeight,
		0xfffffff, false, 3.0f);
}

void UnlickScene::NormalDraw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xfffffff, false, 3.0f);
	DrawString(margin_size + 10, margin_size + 10, L"Pause Scene", 0x0000ff);
	LockPickingDraw();
}

void UnlickScene::LockPickingDraw(void)
{
}
