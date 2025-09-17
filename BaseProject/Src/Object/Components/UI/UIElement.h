#pragma once

class UIElement
{
public:
    virtual ~UIElement(void) = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw(void) = 0;

    virtual int GetSlotItemId(void) const;

private:

    float time_;    // Œo‰ßŽžŠÔ
};

