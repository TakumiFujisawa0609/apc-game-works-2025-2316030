#include "../../../../../Manager/ResourceManager.h"
#include "ItemBase.h"

ItemBase::ItemBase(std::shared_ptr<ActorBase> owner)
	:
	ItemComponent(owner),
	useCount_(0),
	maxUseCount_(0),
	imgId_(-1),
	isOnStage_(false),
	isEquipment_(false),
	isEfficacy_(false),
	isDisabled_(false),
	resMng_(ResourceManager::GetInstance()),
	state_(STATE::NONE)
{
}

int ItemBase::GetImgId(void)
{
	return imgId_;
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
