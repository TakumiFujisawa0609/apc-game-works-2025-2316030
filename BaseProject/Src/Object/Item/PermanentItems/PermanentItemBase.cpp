#include "PermanentItemBase.h"

PermanentItemBase::PermanentItemBase(Player& player)
	:
	ItemBase(player)
{
}

PermanentItemBase::~PermanentItemBase(void)
{
}

bool PermanentItemBase::IsActive(void)
{
	return isActive_;
}

void PermanentItemBase::UpdateState(float deltaTime)
{
	ItemBase::UpdateState(deltaTime);
}

void PermanentItemBase::UpdateOnStage(float deltaTime)
{
}

void PermanentItemBase::UpdateInVentory(float deltaTime)
{
	ItemBase::UpdateInVentory(deltaTime);
}

void PermanentItemBase::UpdateUsed(float deltaTime)
{
}

void PermanentItemBase::UpdateUsedUp(float deltaTime)
{
}
