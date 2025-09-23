#pragma once
#include "ExChangeSlot.h"
class ItemSlot :
    public ExChangeSlot
{
public:
    ItemSlot(int forntItemId, int backItemId);

	void Update(float deltaTime) override;
	void Draw(void) override;

private:


};

