#include <DxLib.h>
#include "../../../../Application.h"
#include "MediumRangeWeaponSlot.h"

MediumRangeWeaponSlot::MediumRangeWeaponSlot(int frontItemId, int backItemId)
	:
	ExChangeSlot(frontItemId, backItemId)
{
	posX_ = static_cast<float>(Application::SCREEN_SIZE_X * 0.45);
	posY_ = static_cast<float>(Application::SCREEN_SIZE_Y * 0.67);
}

MediumRangeWeaponSlot::~MediumRangeWeaponSlot(void)
{
}

void MediumRangeWeaponSlot::Update(float deltaTime)
{
}

void MediumRangeWeaponSlot::Draw(void)
{
	// 枠
	DrawBox(posX_, posY_, posX_ + posX_ * 0.1, posY_ + posY_ * 0.1, GetColor(255, 255, 255), true);

	// アイテムスロット
	int front = ExChangeSlot::GetFront();
	if (front != 0)
	{
		DrawGraph(posX_, posY_, front, true);
	}
}

void MediumRangeWeaponSlot::SetForntSlot(int frontItemId)
{
	int front = ExChangeSlot::GetFront();
	front = frontItemId;
}

void MediumRangeWeaponSlot::SetBackSlot(int backItemId)
{
	int back = ExChangeSlot::GetBack();
	back = backItemId;
}
