#include <functional>
#include <algorithm>
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
	angle_(0.0f),
	lLevel_(-1),
	rotRate_(0.0f),
	isDefault_(false),
	isUnlocking_(false)
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
		ResourceManager::SRC::LOCKPICK_M));
	InitModel(
		INIT_POS,
		INIT_SCL,
		INIT_QUAROTLOCAL);

	// UI画像
	imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::BOTTLE_I);

	angle_ = static_cast<float>(transform_.quaRotLocal.z);

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
	if (IsCurrentSelected() ||
		isUnlocking_)
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

float Lockpick::CalculateRotRate(void)
{

	// 角度の差
	float diffAngle = angle_ - MIN_ANGLE;

	// 割合計算
	float ratio = diffAngle / TOTAL_RANGE;

	return std::clamp(ratio, 0.0f, 1.0f);
}

void Lockpick::SetDefault(void)
{
	// 座標
	transform_.pos = { UNLOCK_POS };

	// ローカル回転
	transform_.quaRotLocal = {
		Quaternion::Euler(
				AsoUtility::Deg2RadF(0.0f),
				AsoUtility::Deg2RadF(0.0f),
				AsoUtility::Deg2RadF(0.0f))
	};

	transform_.Update();

	// 一度だけ呼び出す
	isDefault_ = false;
}

void Lockpick::UpdateUnlock(float deltaTime)
{
	if (isDefault_)
	{
		SetDefault();
	}

	// 回転割合


	transform_.Update();
}

void Lockpick::SetLockLevel(int level)
{
	lLevel_ = level;
}

float Lockpick::GetRotationRate(void)
{
	return rotRate_;
}

bool Lockpick::GetDefault(void)
{
	return isDefault_;
}

void Lockpick::SetIsDefault(bool flag)
{
	isDefault_ = flag;
}

void Lockpick::SetIsUnlocking(bool flag)
{
	isUnlocking_ = flag;
}


