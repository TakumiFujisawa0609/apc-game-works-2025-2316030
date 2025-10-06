#include <functional>
#include "../../../../../Application.h"
#include "../../../../../Common/Quaternion.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Camera.h"
#include "../../../../../Utility/AsoUtility.h"
#include "Radio.h"

Radio::Radio(std::shared_ptr<ActorBase> owner)
	:
	ItemBase(owner)
{
	mName_ = L"Radio";
	itemType_ = ItemType::CONSUMABLE;
}

Radio::~Radio(void)
{
}

void Radio::Init(void)
{
	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::RADIO_M));
	InitModel(
		INIT_POS,
		INIT_SCL,
		INIT_QUAROTLOCAL);

	// UI画像
	imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::RADIO_I);


	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	ChangeState(STATE::ININVENTORY);
}

void Radio::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();

}

void Radio::Draw(void)
{
	if (IsCurrentSelected())
	{
		MV1DrawModel(transform_.modelId);
		return;
	}

}

void Radio::UpdateOnStage(float deltaTime)
{
	// ステージにアイテムが落ちている状態
	// アイテムが拾われる処理


	//拾われるとUpdateInVentoryに遷移
	if (!isOnStage_)
	{
		ChangeState(STATE::ININVENTORY);
	}
}

void Radio::UpdateInVentory(float deltaTime)
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

void Radio::UpdateInUse(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);
}

void Radio::UpdateUsedUp(float deltaTime)
{
}

void Radio::UpdateDisabled(float deltaTime)
{
}
