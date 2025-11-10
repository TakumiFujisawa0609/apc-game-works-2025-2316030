#include "GameScene.h"
#include<DxLib.h>
#include<cassert>
#include<cmath>
#include<random>
#include<chrono>

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
#include"../Object/Enemy/EnemyBase.h"
#include"../Object/Stage/Stage.h"

#include "../Object/Item/PermanentItems/Lockpick.h"
#include "../Object/Item/PermanentItems/HandLight.h"

#include "../Object/Item/ComsumableItems/Battery.h"
#include "../Object/Item/ComsumableItems/Hemostatic.h"
#include "../Object/Item/ComsumableItems/Tranquilizer.h"


#include "../Object/Item/Wire.h"

#include "../Object/Components/Gameplay/OxygenComponent.h"

#include "../Object/Item/ItemStrage/SlotBase.h"

#include "../Object/Components/UI/Components/PlayerStatusUI.h"

#include"../DrawUtil.h"

namespace {
	constexpr int fade_interval = 30;
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
	isHitItem_ = false;
	state_ = STATE::MAINGAME;
	int sw, sh, depth;
	GetScreenState(&sw, &sh, &depth);


	spownPos_.push_back({ -600.0f, 150.0f, 150.0f });
	spownPos_.push_back({ -700.0f, 150.0f, 150.0f });
	spownPos_.push_back({ -800.0f, 150.0f, 150.0f });
	spownPos_.push_back({ -900.0f, 150.0f, 150.0f });
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
	
	// 敵の基底クラス
	eBase_ = std::make_shared<EnemyBase>(*player_);

	stage_ = std::make_shared<Stage>(*player_, *eBase_);

	// ひとつの型でアイテムを管理する
	const int INIT_POOL_SIZE = 6;
	itemPool_.reserve(INIT_POOL_SIZE);
	
	auto light = std::make_shared<HandLight>(*player_);
	light->Init();
	light->SetTargetPos(&player_->GetTransform());
	player_->SetHandLight(light);
	// ステージでてきおうさせるライトを設定する
	stage_->SetCurrentHandLight(light);
	itemPool_.push_back(light);

	auto lockpick = std::make_shared<Lockpick>(*player_);
	lockpick->Init();
	lockpick->SetTargetPos(&player_->GetTransform());
	itemPool_.push_back(lockpick);
	
	// ランダム生成のアイテム
	randomItems_.reserve(4);

	auto wire = std::make_shared<Wire>(*player_);
	wire->Init();
	randomItems_.push_back(wire);

	auto battery = std::make_shared<Battery>(*player_);
	battery->Init();
	battery->SetHandLight(light);
	randomItems_.push_back(battery);

	auto hemostatic = std::make_shared<Hemostatic>(*player_);
	hemostatic->Init();
	randomItems_.push_back(hemostatic);

	auto tranquilizer = std::make_shared<Tranquilizer>(*player_);
	tranquilizer->Init();
	randomItems_.push_back(tranquilizer);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine  generator(seed);

	for (auto& item : randomItems_)
	{
		if (spownPos_.empty())
		{
			break;
		}

		std::uniform_int_distribution<int> distribution(0, spownPos_.size() - 1);
		int randomIndex = distribution(generator);

		VECTOR selectedPos = spownPos_[randomIndex];

		item->SetPos(selectedPos);

		spownPos_.erase(spownPos_.begin() + randomIndex);
		itemPool_.push_back(item);
	}
	randomItems_.clear();

	eBase_->SetNavGridManagedr(stage_->GetNavGridMananger());
	eBase_->Init();

	eBase_->InitComponents();

	stage_->Init();
	stage_->InitRenderer();

	// アイテムスロット
	itemSlot_ = std::make_shared<SlotBase>();

	// ステータス
	status_ = std::make_shared<PlayerStatusUI>(player_, *player_);

	Application::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	Application::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FPS_MOUSE, AsoUtility::VECTOR_ZERO, false);
	isFps_ = true;

	ChangeState(STATE::MAINGAME);
}

void GameScene::Update(Input& input)
{
	float time = Application::GetInstance().GetDeltaTime();
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}

void GameScene::DrawUI(void)
{
	if (IsHitItems())
	{
		const TCHAR* text_to_display = _T("右クリック or Aボタン");
		int text_width = GetDrawStringWidth(text_to_display, static_cast<int>(_tcslen(text_to_display)));

		auto size = Application::GetInstance().GetWindowSize();
		// X座標: 画面中央 (画面幅 / 2) からテキスト幅の半分を引く
		int draw_x = (size.width / 2) - (text_width / 2);

		// Y座標: 画面全体の高さの 4分の3 の位置
		int draw_y = (size.height * 3) / 4;

		// 3. テキストを描画

		// 赤色で描画
		int color = GetColor(255, 255, 255);

		// 描画関数でテキストを表示
		DrawString(draw_x, draw_y, text_to_display, color);
	}

	player_->DrawUI();

	DrawUIItemPool();

	itemSlot_->Draw();

	// プレイヤー状態
	//status_->Draw();
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

	const auto& camera = Application::GetInstance().GetCamera();
	VECTOR prevAngle_ = {};
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_ESCAPE))
	{
		controller_.PushScene(std::make_shared<PauseScene>(controller_), input);
		camera->SetOperableCamera(false);
	}

	if (ins.IsNew(KEY_INPUT_P))
	{
		controller_.ChangeScene(std::make_shared<OverScene>(controller_), input);
		return;
	}

	float time = Application::GetInstance().GetDeltaTime();

	switch (state_)
	{
	case GameScene::STATE::TUTORIAL:
		UpdateTutorial(time, input);
		break;
	case GameScene::STATE::MAINGAME:
		UpdateMainGame(time, input);
		break;
	default:
		break;
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

	switch (state_)
	{
	case GameScene::STATE::TUTORIAL:
		DrawTutorial();
		break;
	case GameScene::STATE::MAINGAME:
		DrawMainGame();
		break;
	default:
		break;
	}

#pragma region MyRegion

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
		int text_width = GetDrawStringWidth(text_to_display, static_cast<int>(_tcslen(text_to_display)));

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

#pragma endregion

	//DrawFormatString(0, 52, GetColor(0, 0, 0), L"cAngle=(%.2f,%.2f,%.2f)", angles_.x, angles_.y, angles_.z);
	//DrawString(10, 0, L"Game Scene", 0xffffff);

}

void GameScene::FadeDraw()
{

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

void GameScene::UpdateItemPool(float deltaTime)
{
	for (auto item : itemPool_)
	{
		item->Update(deltaTime);
	}
}

void GameScene::DrawItemPool(void)
{
	for (const auto& item : itemPool_)
	{
		item->Draw();
	}
}

void GameScene::DrawUIItemPool(void)
{
	for (const auto& item : itemPool_)
	{
		item->DrawUI();
	}
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

void GameScene::UpdateTutorial(float deltaTime,Input& input)
{
	// 衝突判定
	stage_->Update(deltaTime);

	player_->Update(deltaTime);
	eBase_->Update(deltaTime);

	// スロット
	itemSlot_->Update(deltaTime);
	HandleMouseWheel(input);

	UpdateItemPool(deltaTime);

	status_->Update(deltaTime);
}

void GameScene::UpdateMainGame(float deltaTime, Input& input)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgMouseRight() && IsHitItems())
	{
		ObtainItem();
		isHitItem_ = false;
	}

	if (ins.IsTrgDown(KEY_INPUT_E))
	{
		if (itemSlot_)
		{
			itemSlot_->UseCurrentItem();
		}
	}

	// 衝突判定
	stage_->Update(deltaTime);

	player_->Update(deltaTime);
	//eBase_->Update(deltaTime);

	// スロット
	itemSlot_->Update(deltaTime);
	HandleMouseWheel(input);

	UpdateItemPool(deltaTime);

	status_->Update(deltaTime);
}

void GameScene::DrawTutorial(void)
{
	// オブジェクト
	stage_->Draw();
	//eBase_->Draw();
	player_->Draw();

	DrawItemPool();
	player_->DrawUI();

	status_->Draw();
}

void GameScene::DrawMainGame(void)
{

	// オブジェクト
	stage_->Draw();
	player_->Draw();

	DrawItemPool();


	itemSlot_->Draw();
}

void GameScene::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case GameScene::STATE::TUTORIAL:
		ChangeTutorial();
		break;
	case GameScene::STATE::MAINGAME:
		ChangeMainGame();
		break;
	default:
		break;
	}
}

void GameScene::ChangeTutorial(void)
{
}

void GameScene::ChangeMainGame(void)
{
}

void GameScene::ObtainItem(void)
{
	std::shared_ptr<ItemBase> obtainedItem = isObtainItems();

	if (!obtainedItem)
	{
		return;
	}

	if (auto light = std::dynamic_pointer_cast<HandLight>(obtainedItem))
	{
		itemSlot_->AddItem(light);
	}
	if (auto lockpick = std::dynamic_pointer_cast<Lockpick>(obtainedItem))
	{
		itemSlot_->AddItem(lockpick);
	}
	if (auto wire = std::dynamic_pointer_cast<Wire>(obtainedItem))
	{
		itemSlot_->AddItem(wire);
	}
	if (auto battery = std::dynamic_pointer_cast<Battery>(obtainedItem))
	{
		itemSlot_->AddItem(battery);
	}
	if (auto hemostatic = std::dynamic_pointer_cast<Hemostatic>(obtainedItem))
	{
		itemSlot_->AddItem(hemostatic);
	}
	if (auto tranquilizer = std::dynamic_pointer_cast<Tranquilizer>(obtainedItem))
	{
		itemSlot_->AddItem(tranquilizer);
	}
}

std::shared_ptr<ItemBase> GameScene::isObtainItems(void)
{
	const auto& camera = Application::GetInstance().GetCamera();
	VECTOR cPos = camera->GetPos();
	VECTOR tPos = camera->GetTargetPos(); // カメラ注視点

	std::shared_ptr<ItemBase> closestItem = nullptr; // 最も近いアイテムを保持するポインタ
	float minItemDistanceSq = FLT_MAX; // 最小アイテム距離の二乗

	isHitItem_ = false; // ループ開始時にリセット

	for (const auto& item : itemPool_)
	{
		// ONSTAGE状態のアイテムのみを判定
		if (item->GetState() == ItemBase::STATE::ONSTAGE)
		{
			// アイテムとステージの衝突情報を取得
			auto itemHit = MV1CollCheck_Line(item->GetTransform().modelId, -1, cPos, tPos);
			auto stageHit = MV1CollCheck_Line(stage_->GetTransform().modelId, -1, cPos, tPos);

			// 1. アイテムにレイが当たったか
			if (itemHit.HitFlag == 1)
			{
				// カメラからアイテム衝突位置までの距離の二乗を計算
				float itemDistSq = VSquareSize(VSub(itemHit.HitPosition, cPos));

				// カメラからステージ衝突位置までの距離の二乗を計算
				float stageDistSq = VSquareSize(VSub(stageHit.HitPosition, cPos));

				// 2. アイテムがステージよりも手前にあるか、またはステージに当たっていないか
				// (ステージに当たっていない場合は stageHit.HitFlag が 0 で、stageDistSq が大きい値になるはず)

				// **重要な判定:** アイテムへの距離がステージへの距離より小さい、またはステージへの衝突がない
				if (itemDistSq < stageDistSq || stageHit.HitFlag == 0)
				{
					// 有効なアイテム衝突。最も近いアイテムを更新
					if (itemDistSq < minItemDistanceSq)
					{
						minItemDistanceSq = itemDistSq;
						closestItem = item;
					}
				}
			}
		}
	}
	if (closestItem != nullptr)
	{
		isHitItem_ = true;
		return closestItem;

	}

	return nullptr;
}

bool GameScene::IsHitItems(void)
{
	return isObtainItems() != nullptr;
}


