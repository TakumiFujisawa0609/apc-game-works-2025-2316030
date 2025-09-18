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
	void Use(void) override;	// アイテム使用

private:
	float oxygenAmount_; // 回復する酸素量
	float useCooldown_;  // 使用間隔

};

