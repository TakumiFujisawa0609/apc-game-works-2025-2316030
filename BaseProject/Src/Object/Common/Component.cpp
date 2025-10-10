#include "Component.h"

Component::Component(std::shared_ptr<ActorBase> owner)
	:
	mOwner_(owner)
{
}

std::weak_ptr<ActorBase> Component::GetOwner(void) const
{
	return mOwner_.lock();
}
