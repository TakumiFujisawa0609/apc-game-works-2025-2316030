#pragma once

#include "../../Gameplay/Item/Consumable/ItemBase.h"

#include "SlotBase.h"
class ItemSlot :
	public SlotBase
{
public:
    ItemSlot(void);
	~ItemSlot(void);

	void Update(float deltaTime) override;
	void Draw(void) override;
	int GetSelectedUIHandle(void)const;


private:

	float posX_, posY_;

};

