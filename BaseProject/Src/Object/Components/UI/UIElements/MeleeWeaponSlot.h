#pragma once
#include "SlotBase.h"

class MeleeWeaponSlot :
    public SlotBase
{
public:
    MeleeWeaponSlot();
    ~MeleeWeaponSlot(void)override;

    void Update(float deltaTime) override;
    void Draw(void) override;

private:
    float posX_, posY_;
};

