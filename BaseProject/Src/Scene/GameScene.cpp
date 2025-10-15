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
#include "ClearScene.h"
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
	player_->InitComponents();
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

	wire_ = std::make_shared<Wire>(player_);
	wire_->Init();

	// アイテムスロット
	itemSlot_ = std::make_shared<ItemSlot>();
	itemSlot_->AddItem(lockpick_);
	itemSlot_->AddItem(light_);

	// ステータス
	status_ = std::make_shared<PlayerStatusUI>(player_, *player_);

	lockpick_->SetTargetPos(&player_->GetTransform());
	light_->SetTargetPos(&player_->GetTransform());
	Application::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	Application::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FPS_MOUSE, AsoUtility::VECTOR_ZERO, false);
	isFps_ = true;
}

void GameScene::Update(Input& input)
{
	float time = Application::GetInstance().GetDeltaTime();

	// オブジェクト
	stage_->Update(time);
	player_->Update(time);

	// スロット
	itemSlot_->Update(time);
	HandleMouseWheel(input);

	// アイテム
	lockpick_->Update(time);
	light_->Update(time);

	status_->Update(time);

	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();

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

	if (player_->GetTimeLimitComp()->IsTimeDepleted())
	{
		controller_.ChangeScene(std::make_shared<OverScene>(controller_), input);
		return;
	}

	if (wire_->isGameClear())
	{
		controller_.ChangeScene(std::make_shared<ClearScene>(controller_), input);
	}

	const auto& camera = Application::GetInstance().GetCamera();
	VECTOR prevAngle_ = {};
	auto& ins = InputManager::GetInstance();

	VECTOR targetPos = { -2317.0f,189.0f,-1558.0f };
	// 球体1 (標的) の情報
	VECTOR TargetCenter = VGet(-2317.0f,189.0f,-1558.0f);
	const float TargetRadius = 120.0f; // 標的の半径

	// 球体2 (カメラ注視点の代わり、あるいは別の標的) の情報
	VECTOR OtherCenter = camera->GetTargetPos();
	const float OtherRadius = 10.0f; // もう一方の球体の半径

	// 判定に必要な、半径の合計を事前に計算
	const float CombinedRadius = TargetRadius + OtherRadius;
	// 最適化のため、半径の合計の二乗も計算
	const float CombinedRadiusSq = CombinedRadius * CombinedRadius;
	// 1. 中心点間のベクトルの差を計算 (V2 - V1)
	VECTOR DifferenceVector = VSub(OtherCenter, TargetCenter);

	// 2. 中心点間の距離の二乗を計算
	//    VSizeSq はベクトルの長さの二乗を返します。
	float DistanceSq = VSquareSize(DifferenceVector);

	/*DrawSphere3D(TargetCenter, TargetRadius, 8, GetColor(255, 255, 0), GetColor(255, 0, 255), true);
	DrawSphere3D(OtherCenter, OtherRadius, 8, GetColor(255, 0, 255), GetColor(255, 0, 255), true);*/

	// 3. if文による衝突条件の判定
	if (DistanceSq <= CombinedRadiusSq)
	{
		const TCHAR* text_to_display = _T("右クリック or Aボタン");
		int text_width = GetDrawStringWidth(text_to_display, _tcslen(text_to_display));

		auto size = Application::GetInstance().GetWindowSize();
		// X座標: 画面中央 (画面幅 / 2) からテキスト幅の半分を引く
		int draw_x = (size.width / 2) - (text_width / 2);

		// Y座標: 画面全体の高さの 4分の3 の位置
		int draw_y = (size.height * 3) / 4;

		// 3. テキストを描画

		// 赤色で描画
		int color = GetColor(255, 255, 255); // 白にする場合は GetColor(255, 255, 255)

		// 描画関数でテキストを表示
		DrawString(draw_x, draw_y, text_to_display, color);

		if (ins.IsTrgMouseRight() || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			StopSoundMem(player_->GetRunSH());
			StopSoundMem(player_->GetWalkSH());

			std::shared_ptr<UnlickScene> us = std::make_shared<UnlickScene>(controller_);
			us->SetPlayer(player_);
			us->SetWire(wire_);

			us->SetLockPick(lockpick_);

			controller_.PushScene(us, input);
			prevAngle_ = camera->GetAngles();
			camera->SaveAngles(prevAngle_);
			camera->SetOperableCamera(false);
			isFps_ = false;
			return;
		}
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
	lockpick_->Draw();
	light_->Draw();

	// スロット
	itemSlot_->Draw();

	// プレイヤー状態
	status_->Draw();

	const auto& angles_ = Application::GetInstance().GetCamera()->GetAngles();

	const auto& camera = Application::GetInstance().GetCamera();
	VECTOR prevAngle_ = {};
	auto& ins = InputManager::GetInstance();

	VECTOR targetPos = { -2317.0f,189.0f,-1558.0f };
	// 球体1 (標的) の情報
	VECTOR TargetCenter = VGet(-2317.0f, 189.0f, -1558.0f);
	const float TargetRadius = 120.0f; // 標的の半径

	// 球体2 (カメラ注視点の代わり、あるいは別の標的) の情報
	VECTOR OtherCenter = camera->GetTargetPos();
	const float OtherRadius = 10.0f; // もう一方の球体の半径

	// 判定に必要な、半径の合計を事前に計算
	const float CombinedRadius = TargetRadius + OtherRadius;
	// 最適化のため、半径の合計の二乗も計算
	const float CombinedRadiusSq = CombinedRadius * CombinedRadius;
	// 1. 中心点間のベクトルの差を計算 (V2 - V1)
	VECTOR DifferenceVector = VSub(OtherCenter, TargetCenter);

	// 2. 中心点間の距離の二乗を計算
	//    VSizeSq はベクトルの長さの二乗を返します。
	float DistanceSq = VSquareSize(DifferenceVector);

	// 3. if文による衝突条件の判定
	if (DistanceSq <= CombinedRadiusSq)
	{
		const TCHAR* text_to_display = _T("右クリック or Aボタン");
		int text_width = GetDrawStringWidth(text_to_display, _tcslen(text_to_display));

		auto size = Application::GetInstance().GetWindowSize();
		// X座標: 画面中央 (画面幅 / 2) からテキスト幅の半分を引く
		int draw_x = (size.width / 2) - (text_width / 2);

		// Y座標: 画面全体の高さの 4分の3 の位置
		int draw_y = (size.height * 3) / 4;

		// 3. テキストを描画

		// 赤色で描画
		int color = GetColor(255, 255, 255); // 白にする場合は GetColor(255, 255, 255)

		// 描画関数でテキストを表示
		DrawString(draw_x, draw_y, text_to_display, color);
	}


	//DrawFormatString(0, 52, GetColor(0, 0, 0), L"cAngle=(%.2f,%.2f,%.2f)", angles_.x, angles_.y, angles_.z);
	//DrawString(10, 0, L"Game Scene", 0xffffff);

}

void GameScene::FadeDraw()
{
	// オブジェクト
	stage_->Draw();
	player_->Draw();

	lockpick_->Draw();
	light_->Draw();

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
