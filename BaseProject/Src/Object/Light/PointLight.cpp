#include "PointLight.h"

PointLight::PointLight(VECTOR pos, VECTOR color, float intensity, float radius)
	:
	pos_(pos),
	color_(color),
	intensity_(intensity),
	radius_(radius)
{
}

PointLight::~PointLight(void)
{
}

PointLight::LIGHTTYPE PointLight::GetType(void) const
{
	return LIGHTTYPE::POINT;
}

void PointLight::SetPosition(const VECTOR& pos)
{
	pos_ = pos;
}

void PointLight::SetColorAndIntensity(const VECTOR& color, const float& intensity)
{
	color_ = color;
	intensity_ = intensity;
}

void PointLight::SetRadius(const float& radius)
{
	radius_ = radius;
}

const VECTOR& PointLight::GetPointLightPos(void) const
{
	return pos_;
}

const VECTOR& PointLight::GetPointLightColor(void) const
{
	return color_;
}

const float& PointLight::GetPointLightIntensity(void) const
{
	return intensity_;
}

const float& PointLight::GetPointLightRadius(void) const
{
	return radius_;
}
