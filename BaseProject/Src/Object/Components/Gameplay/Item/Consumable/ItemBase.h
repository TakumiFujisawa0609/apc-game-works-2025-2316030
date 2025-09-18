#pragma once

#include "../ItemComponent.h"
class ItemBase :
    public ItemComponent
{
public:

	ItemBase(std::shared_ptr<ActorBase> owner);
	~ItemBase(void) override = default;
	void Init(void) override = 0;
	void Update(float deltaTime) override = 0;
	void Render(void) override = 0;

	virtual void Use(void) = 0;	// アイテム使用

protected:
	int useCount_;  // 使用回数
	int maxUseCount_; // 最大使用回数

};

