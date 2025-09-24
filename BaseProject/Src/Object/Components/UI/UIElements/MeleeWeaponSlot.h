#pragma once
#include<memory>
#include "../../../Components/Gameplay/Item/Weapon/WeaponBase.h"

#include "SlotBase.h"
class MeleeWeaponSlot :
    public SlotBase
{
public:
    MeleeWeaponSlot();
    ~MeleeWeaponSlot(void)override;

    void Update(float deltaTime) override;
    void Draw(void) override;

	// 武器を装備する
	bool Equip(ItemComponent* item);
	
    //武器が装備されているか
    bool isEquipped(void) const;

private:
    // 装備中の武器へのポインタ
	WeaponBase* weapon_;

	// 描画位置
    float posX_, posY_;
};

