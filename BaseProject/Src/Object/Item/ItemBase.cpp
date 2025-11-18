#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "ItemStrage/SlotBase.h"
#include "ItemBase.h"

ItemBase::ItemBase(Player& player)
	:
	player_(player),
	imgH_(-1),
	isOnStage_(false),
	isEquipment_(false),
	isEfficacy_(false),
	isDisabled_(false),
	isInUse_(false),
	isUsed_(false),
	state_(STATE::NONE),
	use_(USE::NONE),
	targetTransform_(nullptr),
	ownerSlot_(),
	slotIndex_(-1)
{
	type_ = ItemBase::TYPE::REGIDBODY;
}

int ItemBase::GetImgId(void)
{
	return imgH_;
}

void ItemBase::SetTargetPos(const Transform* target)
{
	targetTransform_ = target;
}

void ItemBase::SetOwnerSlot(std::shared_ptr<SlotBase> slot, int index)
{
	ownerSlot_ = slot;
	slotIndex_ = index;
}

bool ItemBase::IsCurrentSelected(void) const
{
	if (auto slot = ownerSlot_.lock())
	{
		return slot->GetCurrentSelectedIndex() == slotIndex_;
	}
	return false;
}

void ItemBase::UpdateState(float deltaTime)
{
	switch (state_)
	{
	case ItemBase::STATE::NONE:
		break;
	case ItemBase::STATE::ONSTAGE:
		UpdateOnStage(deltaTime);
		break;
	case ItemBase::STATE::ININVENTORY:
		UpdateInVentory(deltaTime);
		break;
	default:
		break;
	}
}

void ItemBase::UpdateOnStage(float deltaTime)
{
}

void ItemBase::UpdateInVentory(float deltaTime)
{
	// 使用時の処理
	UpdateUsed(deltaTime);
}

void ItemBase::UpdateUsed(float deltaTime)
{
	switch (use_)
	{
	case ItemBase::USE::NONE:
		break;
	case ItemBase::USE::INUSE:
		UpdateInUse(deltaTime);
		break;
	case ItemBase::USE::USEDUP:
		UpdateUsed(deltaTime);
		break;
	default:
		break;
	}
}

void ItemBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state)
	{
	case ItemBase::STATE::NONE:
		break;
	case ItemBase::STATE::ONSTAGE:
		break;
	case ItemBase::STATE::ININVENTORY:
		break;
	default:
		break;
	}
}

void ItemBase::ChangeUse(USE use)
{
	use_ = use;

	switch (use_)
	{
	case ItemBase::USE::NONE:
		break;
	case ItemBase::USE::INUSE:
		break;
	case ItemBase::USE::USEDUP:
		break;
	default:
		break;
	}
}

bool ItemBase::IsDisabledItem(void) const
{
	return isDisabled_;
}

ItemBase::STATE ItemBase::GetState(void) const
{
	return state_;
}

ItemBase::USE ItemBase::GetUse(void) const
{
	return use_;
}

void ItemBase::DrawUI(void)
{
}

void ItemBase::SetPos(VECTOR pos)
{
	transform_.pos = pos;

	// モデルの更新
	transform_.Update();
}

void ItemBase::InitModel(VECTOR pos, VECTOR scl, VECTOR quaRotLocal)
{
	// モデルの設定
	transform_.pos = { pos.x,pos.y,pos.z };
	transform_.scl = { scl.x,scl.y,scl.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(quaRotLocal.x),
		AsoUtility::Deg2RadF(quaRotLocal.y),AsoUtility::Deg2RadF(quaRotLocal.z) });

	// モデル情報の更新
	transform_.Update();
}

void ItemBase::FollowTarget(float deltaTime, VECTOR targetPos)
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
	targetPosOffset = VScale(right, targetPos.x);

	// y軸のオフセット
	targetPosOffset = VAdd(targetPosOffset, VScale(up, targetPos.y));

	// z軸のオフセット
	targetPosOffset = VAdd(targetPosOffset, VScale(forward, targetPos.z));

	// ワールド座標における目標位置
	VECTOR worldTargetPos = VAdd(cameraPos, targetPosOffset);

	transform_.pos = worldTargetPos;

	// 目標１との距離が小さければ強制的に目標位置に設定する
	const float STOP_THRESHOLD = 0.5f;
	if (VSize(VSub(worldTargetPos, transform_.pos)) < STOP_THRESHOLD)
	{
		// 目標１に固定してガタつきをなくす
		transform_.pos = worldTargetPos;

	}
	else {
		// スムース速度
		const float SMOOTH_SPEED = 15.0f;

		// スムースにより滑らかに補間
		float lerpFactor = 1.0f - std::expf(-SMOOTH_SPEED * deltaTime);

		// 現在位置から目標位置へ補間
		transform_.pos = AsoUtility::Lerp(transform_.pos, worldTargetPos, lerpFactor);

	}

	// カメラの注視点を取得
	VECTOR pointPos = Application::GetInstance().GetCamera()->GetTargetPos();

	// 視線ベクトルを求める
	VECTOR lookRotation = VNorm(VSub(pointPos, transform_.pos));

	// 常にカメラの注視点方向をアイテムが向くようにする
	transform_.quaRot = Quaternion::LookRotation(lookRotation, AsoUtility::AXIS_Y);
	transform_.quaRot.y += 0.04f;
}

