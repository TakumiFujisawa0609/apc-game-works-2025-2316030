#include <DxLib.h>
#include "../../../../Application.h"
#include "MeleeWeaponSlot.h"

MeleeWeaponSlot::MeleeWeaponSlot()
{
	weapon_ = nullptr;
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
	// 枠
	DrawBox(posX_, posY_, posX_ + posX_ * 0.1, posY_ + posY_ * 0.1, GetColor(255, 255, 255), true);

	// 近距離武器スロット
	int front = itemId_;
	if (front != 0)
	{
		DrawGraph(posX_, posY_, front, true);
	}
}

bool MeleeWeaponSlot::Equip(ItemComponent* item)
{
	// 近接武器かどうか確認
	if (item->GetItemType() == ItemComponent::ItemType::MELEE_WEAPON)
	{
		weapon_ = static_cast<WeaponBase*>(item);
		return true;
	}
	return false;	// 近接武器ではないので装備できない
}

bool MeleeWeaponSlot::isEquipped(void) const
{
	return weapon_ != nullptr;
}
