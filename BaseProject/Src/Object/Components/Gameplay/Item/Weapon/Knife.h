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
	void Draw(void) override;
	void Release(void) override;
	void Attack(void) override;

private:

	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
};

