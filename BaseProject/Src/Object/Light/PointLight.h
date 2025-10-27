#pragma once
#include "LightBase.h"
class PointLight :
    public LightBase
{
public:
    PointLight(VECTOR pos, VECTOR color, float intensity, float radius);
    ~PointLight(void);

    LIGHTTYPE GetType(void)const override;

    // ワールド座標系での位置を設定
    void SetPosition(const VECTOR& pos);

    // 色と強度を設定
    void SetColorAndIntensity(const VECTOR& color, const float& intensity);

    // 影響範囲を設定
    void SetRadius(const float& radius);

    // 座標参照用
    const VECTOR& GetPointLightPos(void) const;

    // 色参照
    const VECTOR& GetPointLightColor(void) const;

    // 強度参照
    const float& GetPointLightIntensity(void) const;

    // 影響度
    const float& GetPointLightRadius(void) const;

private:

    VECTOR pos_;            // ワールド座標
    VECTOR color_;          // 色
    float intensity_;       // 強度
    float radius_;          // 影響度

};

