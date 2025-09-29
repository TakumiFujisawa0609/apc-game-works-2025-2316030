#pragma once
#include "ItemBase.h"
class OxygenBottle :
    public ItemBase
{
public:

	// í≤êÆópç¿ïW
	static constexpr VECTOR TARGET_POS = { 20.0f,-25.0f, 30.0f };


	OxygenBottle(std::shared_ptr<ActorBase> owner);
	~OxygenBottle(void) override = default;
	void Init(void) override;
	void Update(float deltaTime) override;
	void Draw(void) override;
	void Release(void) override;

private:
	float oxygenAmount_; // âÒïúÇ∑ÇÈé_ëfó 
	float useCooldown_;  // égópä‘äu

	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
	virtual void UpdateUsedUp(float deltaTime) override;
	virtual void UpdateDisabled(float deltaTime) override;
};

