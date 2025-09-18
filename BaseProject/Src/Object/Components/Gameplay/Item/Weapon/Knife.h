#pragma once
#include "WeaponBase.h"
class Knife :
    public WeaponBase
{
public:
	Knife(std::shared_ptr<ActorBase> owner);
	~Knife(void) override = default;
	void Init(void) override;
	void Update(float deltaTime) override;
	void Render(void) override;
	void Attack(void) override;
};

