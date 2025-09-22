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
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.Update();

	// UI画像
	imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::BOTTLE_I);

	// 状態の初期化
	isOnStage_ = true;
}

void OxygenBottle::Update(float deltaTime)
{
	// モデル情報の動機


	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void OxygenBottle::Render(void)
{
	if ((state_ == STATE::ONSTAGE) ||
		(state_ == STATE::INUSE))
	{
		MV1DrawModel(transform_.modelId);
	}
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
	// 現在手に持っているかどうかをスロットを見て判断する


	// 装備しているかどうか
	if (isEquipment_)
	{
		ChangeState(STATE::INUSE);
	}
}

void OxygenBottle::UpdateInUse(float deltaTime)
{
	// 使用中

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


