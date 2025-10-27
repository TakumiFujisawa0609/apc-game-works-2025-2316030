#pragma once
#include "LightBase.h"
class SpotLight :
    public LightBase
{
public:
    SpotLight(VECTOR pos, VECTOR dir, float outAngle, float inAngle, float range);
    LIGHTTYPE GetType(void) const override;

    void SetPosition(const VECTOR& pos);
    void SetDirection(const VECTOR& dir);
    void SetAngles(const float& outer, const float& inner);

    void SetRange(const float& range);
    void SetAttennuation(const float& a0, const float& a1, const float& a2);

    // 座標参照用
    const VECTOR& GetSpotLightPos(void) const;

    // 向き参照
    const VECTOR& GetSpotLightDir(void) const;

    // 外側アングル参照
    const float& GetSpotLightOutAngle(void) const;

    // 内側アングル参照
    const float& GetSpotLightInAngle(void) const;

    // 有効距離参照
    const float& GetSpotLightRange(void) const;

    // atten参照
    const VECTOR& GetSpotLightAtten(void) const;

private:

    VECTOR pos_;            // ワールド座標
    VECTOR dir_;            // 向き
    VECTOR angles_;         // 内と外の角度
    VECTOR atten_;          // atten0,atten1,atten2
    float range_;           // ライトの有効距離

};

