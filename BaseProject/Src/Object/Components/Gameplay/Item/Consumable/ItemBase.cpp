#include "ItemBase.h"

ItemBase::ItemBase(std::shared_ptr<ActorBase> owner)
	:
	ItemComponent(owner),
	useCount_(0),
	maxUseCount_(0)
{
}
