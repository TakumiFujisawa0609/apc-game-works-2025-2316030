#include <DxLib.h>
#include "../../../../Application.h"
#include "MeleeWeaponSlot.h"

MeleeWeaponSlot::MeleeWeaponSlot()
{
	posX_ = static_cast<float>(Application::SCREEN_SIZE_X * 0.45);
	posY_ = static_cast<float>(Application::SCREEN_SIZE_Y * 0.67);
}

MeleeWeaponSlot::~MeleeWeaponSlot(void)
{
}

void MeleeWeaponSlot::Update(float deltaTime)
{

}

void MeleeWeaponSlot::Draw(void)
{
	// òg
	DrawBox(posX_, posY_, posX_ + posX_ * 0.1, posY_ + posY_ * 0.1, GetColor(255, 255, 255), true);

	// ãﬂãóó£ïêäÌÉXÉçÉbÉg
	int front = itemId_;
	if (front != 0)
	{
		DrawGraph(posX_, posY_, front, true);
	}
}
