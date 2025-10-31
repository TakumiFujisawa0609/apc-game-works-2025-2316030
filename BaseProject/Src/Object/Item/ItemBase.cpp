#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "ItemStrage/SlotBase.h"
#include "ItemBase.h"

ItemBase::ItemBase(Player& player)
	:
	player_(player),
	useCount_(0),
	maxUseCount_(0),
	imgH_(-1),
	isOnStage_(false),
	isEquipment_(false),
	isEfficacy_(false),
	isDisabled_(false),
	state_(STATE::NONE),
	targetTransform_(nullptr),
	ownerSlot_(),
	slotIndex_(-1)
{
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
	case ItemBase::STATE::INUSE:
		UpdateInUse(deltaTime);
		break;
	case ItemBase::STATE::USEDUP:
		UpdateUsedUp(deltaTime);
		break;
	case ItemBase::STATE::DISABLED:
		UpdateDisabled(deltaTime);
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
	case ItemBase::STATE::INUSE:
		break;
	case ItemBase::STATE::USEDUP:
		break;
	case ItemBase::STATE::DISABLED:
		break;
	default:
		break;
	}
}

bool ItemBase::IsDisabledItem(void) const
{
	return isDisabled_;
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
}

