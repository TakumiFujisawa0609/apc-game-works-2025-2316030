#include "GameScene.h"
#include<DxLib.h>
#include<cassert>

#include"../Application.h"
#include"../Input.h"//入力用
#include"../Utility/AsoUtility.h"
#include"../Manager/Camera.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"//リソース管理用
#include"../Manager/SceneController.h"//シーンの切り替えに使う
#include"OverScene.h"//次のシーン
#include"PauseScene.h"//ポーズシーン
#include"UnlickScene.h"
#include"../Object/Player/Player.h"
#include"../Object/Stage/Stage.h"

#include "../Object/Player/Inventory.h"

#include "../Object/Components/Gameplay/Item/Consumable/Lockpick.h"
#include "../Object/Components/Gameplay/Item/Consumable/HandLight.h"
#include "../Object/Components/Gameplay/Item/Consumable/Knife.h"
#include "../Object/Components/Gameplay/Item/Consumable/Radio.h"

#include "../Object/Components/Gameplay/Item/Consumable/Wire.h"

#include "../Object/Components/Gameplay/OxygenComponent.h"

#include "../Object/Components/UI/UIElements/ItemSlot.h"

#include "../Object/Components/UI/Components/PlayerStatusUI.h"

#include"../DrawUtil.h"

namespace {
	constexpr int fade_interval = 60;
	float DegreeToRadian(float degree) {
		//0～360→0～2π
		//0～180→0～π
		//÷180 x π
		return (degree * DX_PI_F) / 180.0f;
	}
}

void GameScene::FadeInUpdate(Input& input)
{
	if (--frame_ <= 0) {
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
	}
}

void GameScene::NormalUpdate(Input& input)
{
	++frame_;
	if (input.IsTriggered("ok")) {
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frame_ = 0;
		return;
	}

	const auto& camera = Application::GetInstance().GetCamera();
	VECTOR prevAngle_ = {};
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		std::shared_ptr<UnlickScene> us = std::make_shared<UnlickScene>(controller_);
		us->SetPlayer(player_);

		std::shared_ptr<Wire> wire = std::make_shared<Wire>(player_);
		us->SetWire(wire);

		us->SetLockPick(lockpick_);

		controller_.PushScene(us, input);
		prevAngle_ = camera->GetAngles();
		camera->SaveAngles(prevAngle_);
		camera->SetOperableCamera(false);
		isFps_ = false;
		return;
	}

	//if (!isFps_ &&
	//	camera->GetCameraMode() != Camera::MODE::FPS_MOUSE)
	//{
	//	isFps_ = true;
	//}
	
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<OverScene>(controller_),input);
		return;
	}


}

void GameScene::NormalDraw()
{

	// オブジェクト
	stage_->Draw();
	player_->Draw();
	lockpick_->Draw();
	light_->Draw();
	knife_->Draw();
	radio_->Draw();

	// スロット
	itemSlot_->Draw();

	// プレイヤー状態
	status_->Draw();

	const auto& angles_ = Application::GetInstance().GetCamera()->GetAngles();

	DrawFormatString(0, 52, GetColor(0, 0, 0), L"cAngle=(%.2f,%.2f,%.2f)", angles_.x, angles_.y, angles_.z);
	DrawString(10, 0, L"Game Scene", 0xffffff);

}

void GameScene::FadeDraw()
{
	// オブジェクト
	stage_->Draw();
	player_->Draw();

	lockpick_->Draw();
	light_->Draw();
	knife_->Draw();
	radio_->Draw();

	// スロット
	itemSlot_->Draw();


	// プレイヤー状態
	status_->Draw();

	float rate = static_cast<float>(frame_) /
					static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(rate * 255));
	DrawBox(0, 0, 640, 480, 0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



}

void GameScene::LoadLocationData()
{
	//ヘッダのサイズは固定
	struct Header {
		char signature[4];
		float version;
		int count;
	};
	Header header = {};
	auto fHandle=FileRead_open(L"./location.dat");
	FileRead_read(&header, sizeof(header), fHandle);
	locationData_.resize(header.count);
	for (auto& location : locationData_) {
		uint8_t nameSize = 0;
		FileRead_read(&nameSize, sizeof(nameSize), fHandle);
		location.name.resize(nameSize);
		FileRead_read(location.name.data(), static_cast<int>(location.name.size()), static_cast<int>(fHandle));
		FileRead_read(&location.pos, sizeof(location.pos), static_cast<int>(fHandle));
		FileRead_read(&location.angle, sizeof(location.angle), static_cast<int>(fHandle));
	}
	
	FileRead_close(fHandle);
}

void GameScene::HandleMouseWheel(Input& input)
{
	if (!itemSlot_)
	{
		return;
	}
	
	auto& ins = InputManager::GetInstance();
	static int wheelCounter = 0;
	int wheelDelta = ins.GetWheelDelta();
	wheelCounter += wheelDelta;

	if (wheelCounter > 1)
	{
		// マウスホイール上回転(アイテムを前に戻す/インデックスを減らす)
		itemSlot_->CycleByWheel(true);
		wheelCounter = 0;
	}
	else if (wheelDelta <= -1)
	{
		// マウスホイール下回転(アイテムを後ろに進める/インデックスを増やす)
		itemSlot_->CycleByWheel(false);
		wheelCounter = 0;
	}
}

GameScene::GameScene(SceneController& controller) :Scene(controller)
{

	update_ = &GameScene::FadeInUpdate;
	draw_ = &GameScene::FadeDraw;
	frame_ = fade_interval;
	isFps_ = true;
	imgH_ = -1;

	int sw, sh, depth;
	GetScreenState(&sw, &sh, &depth);


}

GameScene::~GameScene()
{
	
}

void GameScene::Init(Input& input)
{
	// プレイヤー
	player_ = std::make_shared<Player>();
	player_->Init();

	// ステージ
	stage_ = std::make_shared<Stage>(*player_);
	stage_->Init();

	// インベントリ
	inventory_ = std::make_shared<Inventory>(20);

	// 酸素ボンベ
	lockpick_ = std::make_shared<Lockpick>(player_);
	lockpick_->Init();

	// ハンドライト
	light_ = std::make_shared<HandLight>(player_);
	light_->Init();

	// ナイフ
	knife_ = std::make_shared<Knife>(player_);
	knife_->Init();

	// ラジオ
	radio_ = std::make_shared<Radio>(player_);
	radio_->Init();


	// アイテムスロット
	itemSlot_ = std::make_shared<ItemSlot>();
	itemSlot_->AddItem(lockpick_);
	itemSlot_->AddItem(radio_);
	itemSlot_->AddItem(knife_);
	itemSlot_->AddItem(light_);


	// ステータス
	status_ = std::make_shared<PlayerStatusUI>(player_, *player_);


	lockpick_->SetTargetPos(&player_->GetTransform());
	light_->SetTargetPos(&player_->GetTransform());
	knife_->SetTargetPos(&player_->GetTransform());
	radio_->SetTargetPos(&player_->GetTransform());
	Application::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	Application::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FPS_MOUSE, AsoUtility::VECTOR_ZERO, false);
	isFps_ = true;
}

void GameScene::Update(Input& input)
{
	float time = Application::GetInstance().GetDeltaTime();

	if (player_->GetOxygenComp()->IsOxygenDepleted())
	{
		controller_.ChangeScene(std::make_shared<OverScene>(controller_), input);
		return;
	}

	// オブジェクト
	stage_->Update(time);
	player_->Update(time);

	// スロット
	itemSlot_->Update(time);
	HandleMouseWheel(input);

	// アイテム
	lockpick_->Update(time);
	light_->Update(time);
	knife_->Update(time);
	radio_->Update(time);


	status_->Update(time);

	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();

}
