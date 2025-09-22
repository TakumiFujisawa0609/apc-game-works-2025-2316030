#pragma once
#include "ItemBase.h"
class OxygenBottle :
    public ItemBase
{
public:
	OxygenBottle(std::shared_ptr<ActorBase> owner);
	~OxygenBottle(void) override = default;
	void Init(void) override;
	void Update(float deltaTime) override;
	void Render(void) override;

private:
	float oxygenAmount_; // ‰ñ•œ‚·‚éŽ_‘f—Ê
	float useCooldown_;  // Žg—pŠÔŠu

	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
	virtual void UpdateUsedUp(float deltaTime) override;
	virtual void UpdateDisabled(float deltaTime) override;
};

