#include "Area.h"

AreaConnectionPoint::AreaConnectionPoint(VECTOR pos, int id)
	:
	worldPos_(pos),connectedAreaId_(id)
{
}

AreaConnectionPoint::~AreaConnectionPoint(void)
{
}

Area::Area(int id, VECTOR center, float sizeX, float sizeZ)
    :
    id_(id),
    center_(center),
    sizeX_(sizeX),
    sizeZ_(sizeZ)
{
}

Area::~Area(void)
{
}

bool Area::Contains(const VECTOR& pos) const
{
    // Y軸（高さ）は無視し、X軸とZ軸のみで判定
    float halfSizeX = sizeX_ / 2.0f;
    float halfSizeZ = sizeZ_ / 2.0f;

    // エリア中心からの相対座標
    float relativeX = std::abs(pos.x - center_.x);
    float relativeZ = std::abs(pos.z - center_.z);

    // 判定
    return (relativeX <= halfSizeX) && (relativeZ <= halfSizeZ);
}