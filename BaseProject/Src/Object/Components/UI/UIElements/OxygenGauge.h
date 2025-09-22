#pragma once
#include "../UIElement.h"

class OxygenComponent;

class OxygenGauge :
    public UIElement
{
public:
    OxygenGauge(OxygenComponent& oxygen_);
    ~OxygenGauge(void);

    void Update(float deltaTime) override;
    void Draw(void) override;

private:
    OxygenComponent& oxygen_;
    
    // UI•`‰æÀ•W
    float gaugeCenterX_, gaugeCenterY_;     // ƒQ[ƒW•”•ª
    float gaugeRadius_;                     // ƒQ[ƒW”¼Œa
};

