#include "Knife.h"

Knife::Knife(std::shared_ptr<ActorBase> owner)
	:
	WeaponBase(owner)
{
	itemType_ = ItemType::MELEE_WEAPON;
}

void Knife::Init(void)
{
}

void Knife::Update(float deltaTime)
{
}

void Knife::Render(void)
{
}

void Knife::Attack(void)
{
}
