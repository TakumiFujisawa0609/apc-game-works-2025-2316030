#include "Component.h"

Component::Component(std::shared_ptr<ActorBase> owner, const std::wstring name)
	:
	mOwner_(owner),
	mName_(name)
{
}

const std::wstring& Component::GetName(void) const
{
	return mName_;
}

std::weak_ptr<ActorBase> Component::GetOwner(void) const
{
	return mOwner_.lock();
}
