#pragma once
#include "../../Common/Component.h"
class OxygenComponent :
    public Component
{
public:
	OxygenComponent(std::shared_ptr<ActorBase> owner, float max = 100.0f, float depletionRate = 1.0f);
	virtual ~OxygenComponent();
	
	virtual void Update(float deltaTime) override;
	
	// Œ»İ‚Ì_‘f—Ê‚ğæ“¾
	float GetOxygen() const;

	// _‘f—Ê‚ğ‰ñ•œ
	void AddOxygen(float amount);

	// _‘f‚ªØ‚ê‚½‚©‚Ç‚¤‚©
	bool IsOxygenDepleted() const;

	// _‘f‚ÌŒ¸­
	void Consume(float deltaTime);

private:
	float oxygen_;		// Œ»İ‚Ì_‘f—Ê
	float maxOxygen_;	// Å‘å_‘f—Ê
	float depletionRate_; // _‘fÁ”ï—¦i•b‚ ‚½‚è‚ÌŒ¸­—Êj
	bool isDepleted_;	// _‘fØ‚êƒtƒ‰ƒO

};

