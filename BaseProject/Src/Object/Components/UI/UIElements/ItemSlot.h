#pragma once

#include "../../Gameplay/Item/ItemComponent.h"

#include "ExChangeSlot.h"
class ItemSlot :
    public ExChangeSlot
{
public:
    ItemSlot(int frontItemId, int backItemId);
	~ItemSlot(void);

	void Update(float deltaTime) override;
	void Draw(void) override;

	// 前のスロットに格納する
	void SetForntSlot(int frontItemId);

	// 後ろのスロットに格納する
	void SetBackSlot(int backItemId);

private:

	float posX_, posY_;

};

