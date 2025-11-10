#include "ConsumableItemBase.h"

ConsumableItemBase::ConsumableItemBase(Player& player)
	:
	ItemBase(player)
{
}

void ConsumableItemBase::UpdateState(float deltaTime)
{
	ItemBase::UpdateState(deltaTime);
}

void ConsumableItemBase::UpdateOnStage(float deltaTime)
{
	ItemBase::UpdateOnStage(deltaTime);
}

void ConsumableItemBase::UpdateInVentory(float deltaTime)
{
	ItemBase::UpdateInVentory(deltaTime);
}
