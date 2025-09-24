#include "../../../../../Manager/ResourceManager.h"
#include "../../../../Common/Collider.h"
#include "../../../../Common/Capsule.h"
#include "WeaponBase.h"

WeaponBase::WeaponBase(std::shared_ptr<ActorBase> owner)
	:
	ItemComponent(owner),
	imgId_(-1),
	isOnStage_(false),
	isEquipment_(false),
	isEfficacy_(false),
	isDisabled_(false),
	resMng_(ResourceManager::GetInstance()),
	state_(STATE::NONE),
	targetTransform_(nullptr)
{
}

void WeaponBase::SetTargetPos(const Transform* target)
{
	targetTransform_ = target;
}

void WeaponBase::UpdateState(float deltaTime)
{
	switch (state_)
	{
	case WeaponBase::NONE:
		break;
	case WeaponBase::ONSTAGE:
		UpdateOnStage(deltaTime);
		break;
	case WeaponBase::ININVENTORY:
		UpdateInVentory(deltaTime);
		break;
	case WeaponBase::INUSE:
		UpdateInUse(deltaTime);
		break;
	default:
		break;
	}

}

void WeaponBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state)
	{
	case WeaponBase::NONE:
		break;
	case WeaponBase::ONSTAGE:
		break;
	case WeaponBase::ININVENTORY:
		break;
	case WeaponBase::INUSE:
		break;
	default:
		break;
	}
}
