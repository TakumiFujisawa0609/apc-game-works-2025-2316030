#pragma once
#include "../UIElement.h"

class OxygenGauge :
    public UIElement
{
public:
    OxygenGauge(void);
    ~OxygenGauge();

    void Update(float deltaTime) override;
    void Draw(void) override;

private:

};

