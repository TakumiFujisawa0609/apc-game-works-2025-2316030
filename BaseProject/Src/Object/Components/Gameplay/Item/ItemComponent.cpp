#include "../../../Common/Component.h"
#include "ItemComponent.h"

ItemComponent::ItemComponent(std::shared_ptr<ActorBase> owner)
	:
	Component(owner),
	itemId_(0),
	itemType_(ItemType::NONE)
{
}

int ItemComponent::GetItemId(void) const
{
	return itemId_;
}

ItemComponent::ItemType ItemComponent::GetItemType(void) const
{
	return itemType_;
}
