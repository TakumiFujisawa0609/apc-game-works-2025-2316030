#include <DxLib.h>
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneController.h"
#include"../Manager/InputManager.h"
#include"../Manager/Camera.h"
#include"../Application.h"
#include"../Input.h"
#include"GameScene.h"
#include "../Object/Player/Player.h"

#include "../Object/Item/Wire.h"
#include "../Object/Item/PermanentItems/Lockpick.h"

#include "../Object/Components/PuzzleElements/KeyholePlate.h"
#include "../Object/Components/PuzzleElements/Keyhole.h"
#include "UnlockScene.h"

using namespace std;

namespace {
	constexpr int appear_interval = 30;//出現までのフレーム
	constexpr int input_list_row_height = 40;//メニューの１つあたりの高さ
	constexpr int margin_size = 20;//ポーズメニュー枠の余白
}

UnlockScene::UnlockScene(SceneController& controller)
	:
	Scene(controller),
	player_(nullptr),
	wire_(nullptr),
	lockpick_(nullptr),
	frame_(0),
	update_(&UnlockScene::AppearUpdate),
	draw_(&UnlockScene::ProcessDraw)
{

}

UnlockScene::~UnlockScene(void)
{
}

void UnlockScene::Init(Input& input)
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

void UnlockScene::Update(Input& input)
{
	(this->*update_)(input);
}

void UnlockScene::Draw(void)
{
	(this->*draw_)();
}

void UnlockScene::DrawUI(void)
{
	wire_->DrawDebug();
}

void UnlockScene::SetPlayer(std::shared_ptr<Player> player)
{
	player_ = player;
}

void UnlockScene::SetWire(std::shared_ptr<Wire> wire)
{
	wire_ = wire;
}

void UnlockScene::SetLockPick(std::shared_ptr<Lockpick> lPick)
{
	lockpick_ = lPick;
}

void UnlockScene::AppearUpdate(Input& input)
{
	if (++frame_ >= appear_interval) {
		update_ = &UnlockScene::NormalUpdate;
		draw_ = &UnlockScene::NormalDraw;
		return;
	}
}

void UnlockScene::NormalUpdate(Input& input)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsClickMouseLeft()){
		update_ = &UnlockScene::DisappearUpdate;
		draw_ = &UnlockScene::ProcessDraw;
		return;
	}

	// 時間の取得
	float time = Application::GetInstance().GetDeltaTime();

	// 針金のマウス移動による回転
	wire_->UpdateExplore(time);

	// 最大の鍵穴回転角度 (ラジアン) - 例えば 45度
	const float MAX_KEYHOLE_ROT_DEG = 45.0f;
	const float MAX_KEYHOLE_ROT_RAD = AsoUtility::Deg2RadF(MAX_KEYHOLE_ROT_DEG);

	// 鍵穴の回転率を取得 (0.0f 〜 1.0f)
	float rotRate = wire_->GetLockRotationRate();

	// 回転率に基づいて鍵穴の目標回転角度を計算 (ラジアン)
	// 針金が正解に近づくほど、目標回転角度は MAX_KEYHOLE_ROT_RAD に近づく
	float targetKeyholeRot = MAX_KEYHOLE_ROT_RAD * rotRate;

	// 鍵穴に回転を設定
	// Keyhole::SetAngle は z 軸のローカル回転に設定
	keyhole_->SetAngle(targetKeyholeRot);

	keyhole_->OnUpdate(time);

	if (ins.IsTrgDown(KEY_INPUT_A) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)){

		if (wire_->IsDifference()){
			// ロックレベルを設定
			lockpick_->SetLockLevel(wire_->GetLockLevel());

			// ロックピック、鍵穴ともに回転
			lockpick_->SetIsSuccess(true);
			lockpick_->UpdateUnlock(time);


			// クリア判定を出す
			update_ = &UnlockScene::DisappearUpdate;
			draw_ = &UnlockScene::ProcessDraw;
			return;
		}

	}

	//// 指定の角度に到達したら解錠完了
	//lockpick_->UpdateUnlock(time);

	//keyhole_->Update(time);
}

void UnlockScene::DisappearUpdate(Input& input)
{
	if (--frame_ <= 0) {
		lockpick_->SetIsUnlocking(false);
		const auto& camera = Application::GetInstance().GetCamera();
		camera->SetOperableCamera(true);
		controller_.PopScene(input);
		return;
	}
}

void UnlockScene::ProcessDraw()
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

void UnlockScene::NormalDraw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	//白っぽいセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBox(margin_size, margin_size,
		wsize.width - margin_size, wsize.height - margin_size,
		0xfffffff, true);
	wire_->DrawDebug();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//白枠
	DrawBoxAA(static_cast<float>(margin_size), static_cast<float>(margin_size),
		static_cast<float>(wsize.width - margin_size), static_cast<float>(wsize.height - margin_size),
		0xfffffff, false, 3.0f);
	//DrawString(margin_size + 10, margin_size + 10, L"Pause Scene", 0x0000ff);
	LockPickingDraw();
}

void UnlockScene::LockPickingDraw(void)
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
