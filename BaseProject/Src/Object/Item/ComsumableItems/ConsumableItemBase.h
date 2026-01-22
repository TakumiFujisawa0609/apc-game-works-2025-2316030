#pragma once

class Player;

#include "../ItemBase.h"
class ConsumableItemBase :
	public ItemBase
{
public:


	ConsumableItemBase(Player& player);
	~ConsumableItemBase(void) override = default;
	void Init(void) override = 0;
	void Update(float deltaTime) override = 0;
	void Draw(void) override = 0;

	virtual void Use(void) = 0;

protected:

	// ゲーム内空間でのアイテムの更新
	virtual void UpdateState(float deltaTime);
	virtual void UpdateOnStage(float deltaTime);
	virtual void UpdateInVentory(float deltaTime);

	virtual void UpdateInUse(float deltaTime) = 0;
	virtual void UpdateUsedUp(float deltaTime) = 0;


private:

};

