#include <functional>
#include "../../../../../Application.h"
#include "../../../../../Common/Quaternion.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Camera.h"
#include "../../../../../Utility/AsoUtility.h"
#include "Knife.h"

Knife::Knife(std::shared_ptr<ActorBase> owner)
	:
	ItemBase(owner)
{
	itemType_ = ItemType::CONSUMABLE;
}

Knife::~Knife(void)
{
}

void Knife::Init(void)
{
	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::KNIFE_M));
	transform_.scl = { 0.05f,0.05f,0.05f };
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(15.0f), AsoUtility::Deg2RadF(-25.0f), AsoUtility::Deg2RadF(110.0f) });;
	transform_.Update();

	// UI画像
	imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::KNIFE_I);

	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	ChangeState(STATE::ININVENTORY);
}

void Knife::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void Knife::Draw(void)
{
	if (IsCurrentSelected())
	{
		MV1DrawModel(transform_.modelId);
		return;
	}
}

void Knife::UpdateOnStage(float deltaTime)
{
	// ステージにアイテムが落ちている状態
	// アイテムが拾われる処理


	//拾われるとUpdateInVentoryに遷移
	if (!isOnStage_)
	{
		ChangeState(STATE::ININVENTORY);
	}
}

void Knife::UpdateInVentory(float deltaTime)
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

void Knife::UpdateInUse(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);
}

void Knife::UpdateUsedUp(float deltaTime)
{
	// アイテムが今後使用できなくなった場合
}

void Knife::UpdateDisabled(float deltaTime)
{
	// 一時的に使えない状態

	// 時間経過などで使える状態になったとき
	if (!isDisabled_)
	{
		// インベントリへ
		ChangeState(STATE::ININVENTORY);
	}
}
