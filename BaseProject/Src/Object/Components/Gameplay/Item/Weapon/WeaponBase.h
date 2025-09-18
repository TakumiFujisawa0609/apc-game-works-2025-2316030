#pragma once
#include "../ItemComponent.h"
class WeaponBase :
    public ItemComponent
{
public:
	WeaponBase(std::shared_ptr<ActorBase> owner);
	~WeaponBase(void) override = default;
	void Init(void) override = 0;
	void Update(float deltaTime) override = 0;
	void Render(void) override = 0;
	virtual void Attack(void) = 0;	// çUåÇ

private:

};

