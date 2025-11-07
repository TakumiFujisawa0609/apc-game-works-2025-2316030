#include "ConsumableItemBase.h"

ConsumableItemBase::ConsumableItemBase(Player& player)
	:
	ItemBase(player)
{
}

void ConsumableItemBase::UpdateInVentory(float deltaTime)
{
	ItemBase::UpdateInVentory(deltaTime);
}
