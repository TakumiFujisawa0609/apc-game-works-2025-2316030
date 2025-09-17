#pragma once
#include "ExChangeSlot.h"
class ItemSlot :
    public ExChangeSlot
{
public:
    ItemSlot(int frontItemId, int backItemId);
    ~ItemSlot(void)override;

    void Update(float deltaTime) override;
    void Draw(void) override;

private:
};

