#include "../../../Common/Component.h"
#include "ItemComponent.h"

ItemComponent::ItemComponent(std::shared_ptr<ActorBase> owner)
	:
	Component(owner),
	itemId_(0),
	itemType_(ItemType::NONE),
	count_(0)
{
}

int ItemComponent::GetItemId(void) const
{
	return itemId_;
}

int ItemComponent::GetCount(void) const
{
	return count_;
}

ItemComponent::ItemType ItemComponent::GetItemType(void) const
{
	return itemType_;
}

const std::wstring& ItemComponent::GetName(void) const
{
	return mName_;
}
