#include "../../ObjectBase/Charactor.h"
#include "CharactorComponent.h"

CharactorComponent::CharactorComponent(std::shared_ptr<Charactor> owner)
	:
	Component(owner),
	charaOwner_(owner)
{
}

std::weak_ptr<Charactor> CharactorComponent::GetCharactor(void)
{
	return charaOwner_.lock();
}