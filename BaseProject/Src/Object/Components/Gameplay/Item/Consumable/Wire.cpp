#include <random>
#include <chrono>
#include "../../../../../Application.h"
#include "../../../../../Common/Quaternion.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Camera.h"
#include "../../../../../Utility/AsoUtility.h"
#include "Wire.h"

Wire::Wire(std::shared_ptr<ActorBase> owner)
    :
    ItemBase(owner),
    currentAngle_(0.0f),
    goalAngle_(0.0f)
{
}

Wire::~Wire()
{
}

void Wire::Init(void)
{
    transform_.SetModel(resMng_.LoadModelDuplicate(
        ResourceManager::SRC::KNIFE_M));
    InitModel(
        INIT_POS,
        INIT_SCL,
        INIT_QUAROTLOCAL);

    // UI画像
    imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::KNIFE_I);

    // 現在の角度
    currentAngle_ = transform_.quaRotLocal.z;

    // 正解の角度
    static std::mt19937 generator(
        static_cast<unsigned int>(
            std::chrono::system_clock::now().time_since_epoch().count()
            )
    );
    // 2. 0から180までの整数（int）の均等な分布を定義
    std::uniform_int_distribution<int> distribution(0, 180);

    // 3. 乱数を生成し、float型の目標角度に代入
    // intで生成してからfloatにキャストすることで、0.0f〜180.0fの値を設定します。
    goalAngle_ = static_cast<float>(distribution(generator));

    ChangeState(STATE::INUSE);
}

void Wire::Update(float deltaTime)
{
    ItemBase::UpdateState(deltaTime);

    transform_.Update();
}

void Wire::Draw(void)
{

    DrawFormatString(0, 64, GetColor(0, 0, 0), L"gAngle=(%.2f)", goalAngle_);
}

float Wire::GetCurrentAngle(void)
{
    return currentAngle_;
}

float Wire::GetGoalAngle(void)
{
    return goalAngle_;
}

void Wire::UpdateOnStage(float deltaTime)
{
}

void Wire::UpdateInVentory(float deltaTime)
{
}

void Wire::UpdateInUse(float deltaTime)
{
}

void Wire::UpdateUsedUp(float deltaTime)
{
}

void Wire::UpdateDisabled(float deltaTime)
{
}
