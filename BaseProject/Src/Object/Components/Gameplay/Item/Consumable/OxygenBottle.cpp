#include <functional>
#include "../../../../../Application.h"
#include "../../../../../Common/Quaternion.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Camera.h"
#include "../../../../../Utility/AsoUtility.h"
#include "OxygenBottle.h"

OxygenBottle::OxygenBottle(std::shared_ptr<ActorBase> owner)
	:
	ItemBase(owner),
	oxygenAmount_(0.0f),
	useCooldown_(0.0f)
{
	itemType_ = ItemType::CONSUMABLE;
}

void OxygenBottle::Init(void)
{
	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BOTTLE_M));
	transform_.scl = { 10.0f,10.0f,10.0f };
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.Update();

	// UI画像
	imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::BOTTLE_I);

	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	ChangeState(STATE::ININVENTORY);
}

void OxygenBottle::Update(float deltaTime)
{
	// モデル情報の動機


	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void OxygenBottle::Draw(void)
{
	if ((state_ == STATE::ONSTAGE) ||
		(state_ == STATE::INUSE))
	{
		MV1DrawModel(transform_.modelId);
	}

	MV1DrawModel(transform_.modelId);

	DrawSphere3D(transform_.pos, 10.0f, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
}

void OxygenBottle::Release(void)
{
}

void OxygenBottle::UpdateOnStage(float deltaTime)
{
	// ステージにアイテムが落ちている状態
	// アイテムが拾われる処理
	

	//拾われるとUpdateInVentoryに遷移
	if (!isOnStage_)
	{
		ChangeState(STATE::ININVENTORY);
	}
}

void OxygenBottle::UpdateInVentory(float deltaTime)
{
	// インベントリに格納されている状態
	// 場所の調整
	const auto& camera = Application::GetInstance().GetCamera();
	VECTOR cameraPos = camera->GetPos();
	VECTOR forward = camera->GetForward();
	VECTOR right = camera->GetRight();
	VECTOR up = camera->GetUp();

	VECTOR targetPosOffset = {};

	// x軸のオフセット
	targetPosOffset = VScale(right, TARGET_POS.x);

	// y軸のオフセット
	targetPosOffset = VAdd(targetPosOffset, VScale(up, TARGET_POS.y));

	// z軸のオフセット
	targetPosOffset = VAdd(targetPosOffset, VScale(forward, TARGET_POS.z));

	// ワールド座標における目標位置
	VECTOR targetPos = VAdd(cameraPos, targetPosOffset);

	transform_.pos = targetPos;

	// 目標１との距離が小さければ強制的に目標位置に設定する
	const float STOP_THRESHOLD = 0.5f;
	if (VSize(VSub(targetPos, transform_.pos)) < STOP_THRESHOLD)
	{
		// 目標１に固定してガタつきをなくす
		transform_.pos = targetPos;
		
	}
	else {
		// スムース速度
		const float SMOOTH_SPEED = 15.0f;

		// スムースにより滑らかに補間
		float lerpFactor = 1.0f - std::expf(-SMOOTH_SPEED * deltaTime);

		// 現在位置から目標位置へ補間
		transform_.pos = AsoUtility::Lerp(transform_.pos, targetPos, lerpFactor);

	}
	
	// アイテムの回転をカメラの向きに合わせる
	transform_.quaRot = camera->GetQuaRotOutX();

	// 現在手に持っているかどうかをスロットを見て判断する
	// 持っていれば


	// 装備しているかどうか
	if (isEquipment_)
	{
		ChangeState(STATE::INUSE);
	}
}

void OxygenBottle::UpdateInUse(float deltaTime)
{
	// 使用中
	// 画面のどこかにゲージが出る

	// 使用したときの効果(バフ、デバフなど)

	// アイテム使用されてアイテムとしての効果を失ったとき
	if (!isEfficacy_)
	{
		ChangeState(STATE::USEDUP);
	}

	// 一時的に使用できない状態に陥った場合
	if (isDisabled_)
	{
		ChangeState(STATE::DISABLED);
	}
}

void OxygenBottle::UpdateUsedUp(float deltaTime)
{
	// アイテムが今後使用できなくなった場合

}

void OxygenBottle::UpdateDisabled(float deltaTime)
{
	// 一時的に使えない状態

	// 時間経過などで使える状態になったとき
	if (!isDisabled_)
	{
		// インベントリへ
		ChangeState(STATE::ININVENTORY);
	}

}


