#pragma once
#include "../UIElement.h"

class TimeLimitComponent;

class TimeGauge :
    public UIElement
{
public:
    TimeGauge(TimeLimitComponent& tLimit);
    ~TimeGauge(void);

    void Update(float deltaTime) override;
    void Draw(void) override;

private:
    TimeLimitComponent& tLimit_;        // ŠÔ§ŒÀ
    
    // UI•`‰æÀ•W
    float gaugeCenterX_, gaugeCenterY_;     // ƒQ[ƒW•”•ª
    float gaugeRadius_;                     // ƒQ[ƒW”¼Œa
};

