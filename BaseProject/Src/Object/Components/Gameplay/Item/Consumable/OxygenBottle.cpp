#include "OxygenBottle.h"

OxygenBottle::OxygenBottle(std::shared_ptr<ActorBase> owner)
	:
	ItemBase(owner)
{
	itemType_ = ItemType::CONSUMABLE;
}

void OxygenBottle::Init(void)
{
}

void OxygenBottle::Update(float deltaTime)
{
}

void OxygenBottle::Render(void)
{
}

void OxygenBottle::Use(void)
{
}
