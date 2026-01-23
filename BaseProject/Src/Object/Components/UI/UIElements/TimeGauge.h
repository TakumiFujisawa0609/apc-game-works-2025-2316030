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

    // ŠÔ§ŒÀ
    TimeLimitComponent& tLimit_;
    
    // UI•`‰æÀ•W
    // ƒQ[ƒW•”•ª
    float gaugeCenterX_, gaugeCenterY_;

    // ƒQ[ƒW”¼Œa
    float gaugeRadius_;

};

