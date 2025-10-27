#include "SpotLight.h"

SpotLight::SpotLight(VECTOR pos, VECTOR dir, 
    float outAngle, float inAngle, float range)
    :
    pos_(pos),
    dir_(dir),
    angles_{ outAngle,inAngle,0.0f },
    range_(range)
{
}

SpotLight::LIGHTTYPE SpotLight::GetType(void) const
{
    return LIGHTTYPE::SPOT;
}

void SpotLight::SetPosition(const VECTOR& pos)
{
    pos_ = pos;
}

void SpotLight::SetDirection(const VECTOR& dir)
{
    dir_ = dir;
}

void SpotLight::SetAngles(const float& outer, const float& inner)
{
    angles_ = { outer,inner,0.0f };
}

void SpotLight::SetRange(const float& range)
{
    range_ = range;
}

void SpotLight::SetAttennuation(const float& a0, const float& a1, const float& a2)
{
    atten_ = { a0,a1,a2 };
}

const VECTOR& SpotLight::GetSpotLightPos(void) const
{
    return pos_;
}

const VECTOR& SpotLight::GetSpotLightDir(void) const
{
    return dir_;
}

const float& SpotLight::GetSpotLightOutAngle(void) const
{
    return angles_.x;
}

const float& SpotLight::GetSpotLightInAngle(void) const
{
    return angles_.y;
}

const float& SpotLight::GetSpotLightRange(void) const
{
    return range_;
}

const VECTOR& SpotLight::GetSpotLightAtten(void) const
{
    return atten_;
}
