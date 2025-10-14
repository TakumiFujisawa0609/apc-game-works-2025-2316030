#include <DxLib.h>
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneController.h"
#include"../Manager/InputManager.h"
#include"../Manager/Camera.h"
#include"../Application.h"
#include"../Input.h"
#include"GameScene.h"
#include "../Object/Player/Player.h"
#include "../Object/Components/Gameplay/Item/Consumable/Wire.h"
#include "../Object/Components/Gameplay/Item/Consumable/Lockpick.h"
#include "../Object/Components/PuzzleElements/KeyholePlate.h"
#include "../Object/Components/PuzzleElements/Keyhole.h"
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
	player_(nullptr),
	wire_(nullptr),
	lockpick_(nullptr),
	frame_(0),
	update_(&UnlickScene::AppearUpdate),
	draw_(&UnlickScene::ProcessDraw)
{

}

UnlickScene::~UnlickScene(void)
{
}

void UnlickScene::Init(Input& input)
{
	// 針金の初期化
	wire_->Init();
	wire_->SetIsDefault(true);

	// 鍵穴プレートの初期化
	keyholePlate_ = std::make_shared<KeyholePlate>();
	keyholePlate_->Init();

	// 鍵穴の初期化
	keyhole_ = std::make_shared<Keyhole>();
	keyhole_->Init();
	keyhole_->SetAngle(static_cast<float>(lockpick_->GetTransform().quaRotLocal.z));
	
	// ロックピックの初期化
	lockpick_->SetIsDefault(true);
	lockpick_->SetIsUnlocking(true);
}

void UnlickScene::Update(Input& input)
{
	(this->*update_)(input);
}

void UnlickScene::Draw(void)
{
	(this->*draw_)();
}

void UnlickScene::SetPlayer(std::shared_ptr<Player> player)
{
	player_ = player;
}

void UnlickScene::SetWire(std::shared_ptr<Wire> wire)
{
	wire_ = wire;
}

void UnlickScene::SetLockPick(std::shared_ptr<Lockpick> lPick)
{
	lockpick_ = lPick;
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

	if (ins.IsTrgDown(KEY_INPUT_SPACE)) 
	{
		update_ = &UnlickScene::DisappearUpdate;
		draw_ = &UnlickScene::ProcessDraw;
		return;
	}

	// 時間の取得
	float time = Application::GetInstance().GetDeltaTime();

	// 針金のマウス移動による回転
	wire_->UpdateExplore(time);
	
	if (ins.IsTrgDown(KEY_INPUT_A)|| ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{

		if (wire_->IsDifference())
		{
			// ロックレベルを設定
			lockpick_->SetLockLevel(wire_->GetLockLevel());
			
			// ロックピック、鍵穴ともに回転
			lockpick_->SetIsSuccess(true);
			lockpick_->UpdateUnlock(time);


			// クリア判定を出す
			update_ = &UnlickScene::DisappearUpdate;
			draw_ = &UnlickScene::ProcessDraw;
			return;
		}

	}
	


	//// 指定の角度に到達したら解錠完了
	//lockpick_->UpdateUnlock(time);

	//keyhole_->Update(time);
}

void UnlickScene::DisappearUpdate(Input& input)
{
	if (--frame_ <= 0) {
		lockpick_->SetIsUnlocking(false);
		const auto& camera = Application::GetInstance().GetCamera();
		camera->SetOperableCamera(true);
		controller_.PopScene(input);
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

	frameHalfHeight *= static_cast<int>(rate);

	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(margin_size, centerY - frameHalfHeight,
		wsize.width - margin_size, centerY + frameHalfHeight,
		0xfffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(static_cast<float>(margin_size), static_cast<float>(centerY - frameHalfHeight),
		static_cast<float>(wsize.width - margin_size), static_cast<float>(centerY + frameHalfHeight),
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
	DrawBoxAA(static_cast<float>(margin_size), static_cast<float>(margin_size),
		static_cast<float>(wsize.width - margin_size), static_cast<float>(wsize.height - margin_size),
		0xfffffff, false, 3.0f);
	//DrawString(margin_size + 10, margin_size + 10, L"Pause Scene", 0x0000ff);
	LockPickingDraw();
}

void UnlickScene::LockPickingDraw(void)
{

	// 鍵穴
	keyhole_->Draw();

	// 鍵枠
	keyholePlate_->Draw();

	// ロックピック
	lockpick_->Draw();

	// 針金
	wire_->Draw();
	
}
