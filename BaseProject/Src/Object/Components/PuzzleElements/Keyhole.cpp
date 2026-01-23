#include <cmath>
#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "Keyhole.h"

Keyhole::Keyhole(void)
    :
    isSuccess_(false)
{
}

Keyhole::~Keyhole(void)
{
}

void Keyhole::Init(void)
{
    // 使用する画像の初期化
    imgH_ = resMng_.Load(ResourceManager::SRC::KEYHOLE).handleId_;

    // 2D情報の初期化
    InitImg(0.0f, 0.0f, 0.25f, 0.0f);
}

void Keyhole::OnUpdate(float deltaTime)
{
    // 現在の回転角度
    float currentRotZ = angle_;
    float goalRotZ = goalAngle_; // 新しい目標角度

    const float LERP_RATE = 5.0f * deltaTime; // deltaTimeを考慮して速度を固定

    // Lerpで回転
    float diff = goalRotZ - currentRotZ;

    if (std::abs(diff) > 0.001f){
        float delta = diff * LERP_RATE;
        // 目標を超えないように補正
        if (std::abs(delta) > std::abs(diff)) { delta = diff; }
        currentRotZ += delta;
    }
    else{
        currentRotZ = goalRotZ;
    }

    angle_ = currentRotZ;
}

void Keyhole::Draw(void)
{
    PlazzleElementBase::Draw();
}

void Keyhole::SetAngle(float angle)
{
    goalAngle_ = angle;
}

void Keyhole::SetIsSuccess(bool isSuccess)
{
    isSuccess_ = isSuccess;
}

bool Keyhole::GetIsSuccess(void) const
{
    return isSuccess_;
}
