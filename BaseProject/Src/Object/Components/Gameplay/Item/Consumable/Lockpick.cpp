#include <functional>
#include "../../../../../Application.h"
#include "../../../../../Common/Quaternion.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Camera.h"
#include "../../../../../Utility/AsoUtility.h"
#include "Lockpick.h"

Lockpick::Lockpick(std::shared_ptr<ActorBase> owner)
	:
	ItemBase(owner),
	isUse_(false),
	angles_(0.0f)
{
	mName_ = L"LockPick";
	itemType_ = ItemType::CONSUMABLE;
}

Lockpick::~Lockpick(void)
{
}

void Lockpick::Init(void)
{
	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BOTTLE_M));
	InitModel(
		INIT_POS,
		INIT_SCL,
		INIT_QUAROTLOCAL);

	// UI画像
	imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::BOTTLE_I);


	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	ChangeState(STATE::ININVENTORY);
}

void Lockpick::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void Lockpick::Draw(void)
{
	if (IsCurrentSelected())
	{
		MV1DrawModel(transform_.modelId);
		return;
	}


}

bool Lockpick::IsUse(bool use)
{
	isUse_ = use;
	return isUse_;
}

void Lockpick::UpdateOnStage(float deltaTime)
{
	// ステージにアイテムが落ちている状態
	// アイテムが拾われる処理
	

	//拾われるとUpdateInVentoryに遷移
	if (!isOnStage_)
	{
		ChangeState(STATE::ININVENTORY);
	}
}

void Lockpick::UpdateInVentory(float deltaTime)
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

void Lockpick::UpdateInUse(float deltaTime)
{
	// プレイヤーに追尾
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	if (isUse_)
	{

	}
}

void Lockpick::UpdateUsedUp(float deltaTime)
{
	// アイテムが今後使用できなくなった場合

}

void Lockpick::UpdateDisabled(float deltaTime)
{
	// 一時的に使えない状態

	// 時間経過などで使える状態になったとき
	if (!isDisabled_)
	{
		// インベントリへ
		ChangeState(STATE::ININVENTORY);
	}

}

void Lockpick::UpdateUnlock(float deltaTime)
{
}


