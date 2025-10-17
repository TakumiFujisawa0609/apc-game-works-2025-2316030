#include <functional>
#include "../../../../../Application.h"
#include "../../../../../Common/Quaternion.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Camera.h"
#include "../../../../../Utility/AsoUtility.h"
#include "HandLight.h"

HandLight::HandLight(std::shared_ptr<ActorBase> owner)
	:
	ItemBase(owner)
{
	mName_ = L"HandLight";
	itemType_ = ItemType::CONSUMABLE;
}

HandLight::~HandLight(void)
{
}

void HandLight::Init(void)
{

	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FLASHLIGHT_M));
	InitModel(
		INIT_POS,
		INIT_SCL,
		INIT_QUAROTLOCAL);

	// UI画像
	imgH_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::FLASHLIGHT_I);


	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	ChangeState(STATE::ININVENTORY);
}

void HandLight::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void HandLight::Draw(void)
{

	if (IsCurrentSelected())
	{
		MV1DrawModel(transform_.modelId);
		return;
	}
}

void HandLight::UpdateOnStage(float deltaTime)
{
	// ステージにアイテムが落ちている状態
	// アイテムが拾われる処理


	//拾われるとUpdateInVentoryに遷移
	if (!isOnStage_)
	{
		ChangeState(STATE::ININVENTORY);
	}
}

void HandLight::UpdateInVentory(float deltaTime)
{
	// 追従
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	// 現在手に持っているかどうかをスロットを見て判断する
	// 持っていれば


	// 装備しているかどうか
	if (isEquipment_)
	{
		ChangeState(STATE::INUSE);
	}
}

void HandLight::UpdateInUse(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);
}

void HandLight::UpdateUsedUp(float deltaTime)
{
	// アイテムが今後使用できなくなった場合

}

void HandLight::UpdateDisabled(float deltaTime)
{
	// 一時的に使えない状態

	// 時間経過などで使える状態になったとき
	if (!isDisabled_)
	{
		// インベントリへ
		ChangeState(STATE::ININVENTORY);
	}
}


