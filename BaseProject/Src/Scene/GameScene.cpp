#include "GameScene.h"
#include<DxLib.h>
#include<cassert>

#include"../Application.h"
#include"../Input.h"//入力用
#include"../Manager/Camera.h"
#include"../Manager/ResourceManager.h"//リソース管理用
#include"../Manager/SceneController.h"//シーンの切り替えに使う
#include"OverScene.h"//次のシーン
#include"PauseScene.h"//ポーズシーン
#include"../Object/Player/Player.h"
#include"../Object/Stage/Stage.h"

#include "../Object/Player/Inventory.h"

#include "../Object/Components/Gameplay/Item/Consumable/OxygenBottle.h"
#include "../Object/Components/Gameplay/Item/Weapon/Knife.h"

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
	if (input.IsTriggered("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}


	
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
	oxygenBottle_->Draw();
	knife_->Draw();

	// スロット
	itemSlot_->Draw();

	// プレイヤー状態
	status_->Draw();


	DrawString(10, 0, L"Game Scene", 0xffffff);

}

void GameScene::FadeDraw()
{
	// オブジェクト
	stage_->Draw();
	player_->Draw();

	oxygenBottle_->Draw();
	knife_->Draw();

	// スロット
	itemSlot_->Draw();


	// プレイヤー状態
	status_->Draw();

	float rate = static_cast<float>(frame_) /
					static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate * 255);
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
		FileRead_read(location.name.data(), location.name.size(), fHandle);
		FileRead_read(&location.pos, sizeof(location.pos), fHandle);
		FileRead_read(&location.angle, sizeof(location.angle), fHandle);
	}
	
	FileRead_close(fHandle);
}

void GameScene::HandleMouseWheel(int wheelDelta)
{
	if (!itemSlot_)
	{
		return;
	}
	

	if (wheelDelta > 0)
	{
		// マウスホイール上回転(アイテムを前に戻す/インデックスを減らす)
		itemSlot_->CycleByWheel(true);

	}
	else if (wheelDelta < 0)
	{
		// マウスホイール下回転(アイテムを後ろに進める/インデックスを増やす)
		itemSlot_->CycleByWheel(false);
	}
}

GameScene::GameScene(SceneController& controller) :Scene(controller)
{

	update_ = &GameScene::FadeInUpdate;
	draw_ = &GameScene::FadeDraw;
	frame_ = fade_interval;
	
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
	stage_ = std::make_shared<Stage>();
	stage_->Init();

	// インベントリ
	inventory_ = std::make_shared<Inventory>(20);

	// 酸素ボンベ
	oxygenBottle_ = std::make_unique<OxygenBottle>(player_);
	oxygenBottle_->Init();

	// ナイフ
	knife_ = std::make_shared<Knife>(player_);
	knife_->Init();

	// アイテムスロット
	itemSlot_ = std::make_shared<ItemSlot>();
	itemSlot_->AddItem(oxygenBottle_);// ボトルを格納

	// 武器スロット

	// ステータス
	status_ = std::make_shared<PlayerStatusUI>(player_, *player_);


	oxygenBottle_->SetTargetPos(&player_->GetTransform());
	Application::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	Application::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FPS_MOUSE);
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
	oxygenBottle_->Update(time);
	knife_->Update(time);


	itemSlot_->Update(time);
	status_->Update(time);

	HandleMouseWheel(GetMouseWheelRotVol());

	(this->*update_)(input);

}

void GameScene::Draw()
{
	(this->*draw_)();

}
