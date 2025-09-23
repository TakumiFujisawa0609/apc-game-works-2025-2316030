#pragma once
#include <string>
#include "../UIElement.h"
class SlotBase :
    public UIElement
{
public:
    SlotBase(void);
    virtual ~SlotBase(void) = default;

    void SetItem(int id);

    void Update(float deltaTime) override;
    void Draw(void) override;

protected:
    int itemId_;
    std::wstring iconPath_;
};

