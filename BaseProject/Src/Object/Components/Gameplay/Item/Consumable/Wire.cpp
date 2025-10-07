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
    goalAngle_(0.0f),
    lastMousePosX_(-1),
    isDifference_(false)
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
    currentAngle_ = static_cast<float>(transform_.quaRotLocal.z);

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
    auto& size = Application::GetInstance().GetWindowSize();
    DrawFormatString(size.width - 200, 64, GetColor(255, 255, 255), L"gAngle=(%.2f)", goalAngle_);
    DrawFormatString(size.width - 200, 80, GetColor(255, 255, 255), L"cAngle_=(%.2f)", currentAngle_);
    DrawFormatString(size.width - 200, 96, GetColor(255, 255, 255), L"rLocal_=(%.2f)", transform_.quaRotLocal.z);
}

float Wire::GetCurrentAngle(void)
{
    return currentAngle_;
}

float Wire::GetGoalAngle(void)
{
    return goalAngle_;
}

void Wire::UpdateExplore(float deltaTime)
{
    currentAngle_ = static_cast<float>(transform_.quaRotLocal.z);

    // マウスをx方向に動かしてtrasnform_.quaRot.zを動かす
    int mx, my;
    GetMousePoint(&mx, &my);

    // 前フレームからの移動量の差分
    float deltaX = static_cast<float>(mx - lastMousePosX_);

    // マウスによるz軸のローカル回転
    transform_.quaRotLocal.z += -deltaX * MOUSE_SENSITIVITY;

    // ローカル回転の制限
    float thresholdRotZ = static_cast<float>(transform_.quaRotLocal.z);
    if (thresholdRotZ > MAX_ROT_Z)
    {
        thresholdRotZ = MAX_ROT_Z;
    }
    else if (thresholdRotZ < MIN_ROT_Z)
    {
        thresholdRotZ = MIN_ROT_Z;
    }

    // マウス座標の更新
    //mx = lastMousePosX_;
    lastMousePosX_ = mx;

    // 判定の更新
    transform_.quaRotLocal.z = static_cast<double>(thresholdRotZ);
    currentAngle_ = static_cast<float>(transform_.quaRotLocal.z);

    // モデルの更新
    transform_.Update();
}

bool Wire::IsDifference(void)
{
    float diffAngle_ = goalAngle_ - currentAngle_;
    if (diffAngle_ == 0.0f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Wire::GetLockLevel(void)
{
    float diffAngle = goalAngle_ - currentAngle_;

    // 20度
    if (diffAngle < FIRST_LOCK)
    {
        return 1;

        // 15度
        if (diffAngle < SECOND_LOCK)
        {
            return 2;

            // 8度
            if (diffAngle < THIRD_LOCK)
            {
                return 3;
            }
        }
    }
    else
    {
        return 0;
    }
    
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
