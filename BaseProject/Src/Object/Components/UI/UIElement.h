#pragma once

class UIElement
{
public:
	UIElement(void);
    virtual ~UIElement(void) = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw(void) = 0;


private:
};

