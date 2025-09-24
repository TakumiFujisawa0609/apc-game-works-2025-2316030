#pragma once
#include "ExChangeSlot.h"
class MediumRangeWeaponSlot :
    public ExChangeSlot
{
public:
	MediumRangeWeaponSlot(int frontItemId, int backItemId);
	~MediumRangeWeaponSlot(void);

	void Update(float deltaTime) override;
	void Draw(void) override;

	// 前のスロットに格納する
	void SetForntSlot(int frontItemId);

	// 後ろのスロットに格納する
	void SetBackSlot(int backItemId);

private:

	float posX_, posY_;
};

