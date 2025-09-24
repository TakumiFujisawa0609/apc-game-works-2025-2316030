#pragma once
#include "ItemBase.h"
class OxygenBottle :
    public ItemBase
{
public:

	// ’²®—pÀ•W
	static constexpr VECTOR TARGET_POS = { 5.0f,5.0f, 50.0f };


	OxygenBottle(std::shared_ptr<ActorBase> owner);
	~OxygenBottle(void) override = default;
	void Init(void) override;
	void Update(float deltaTime) override;
	void Draw(void) override;

private:
	float oxygenAmount_; // ‰ñ•œ‚·‚é_‘f—Ê
	float useCooldown_;  // g—pŠÔŠu

	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
	virtual void UpdateUsedUp(float deltaTime) override;
	virtual void UpdateDisabled(float deltaTime) override;
};

