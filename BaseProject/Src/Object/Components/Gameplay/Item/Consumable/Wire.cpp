#include <random>
#include <chrono>
#include <cmath>
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
    isDifference_(false),
    lockLevel_(-1),
    isDefault_(false)
{
}

Wire::~Wire()
{
}

void Wire::Init(void)
{
    // モデル情報
    transform_.SetModel(resMng_.LoadModelDuplicate(
        ResourceManager::SRC::WIRE_M));

    auto camera = Application::GetInstance().GetCamera();
    // ① カメラの位置から
    VECTOR camPos = camera->GetPos();

    // ② カメラの「前方ベクトル」（GetForward）を計算し
    VECTOR forward = camera->GetForward();

    // ③ 前方に適切な距離（例: 300.0f）だけ進んだ地点をモデルの位置とする
    // この位置が、カメラの向きにかかわらず画面中央になります
    VECTOR modelPos = VAdd(camPos, VScale(forward, 200.0f));

    transform_.pos = modelPos;
    transform_.scl = { 100.0f,100.0f,100.0f };
    transform_.quaRot = camera->GetQuaRot();

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
    MV1DrawModel(transform_.modelId);

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
    if (isDefault_)
    {
        SetDefault();
    }

    // 現在の角度を更新
    currentAngle_ = static_cast<float>(transform_.quaRotLocal.x);

    // マウスをx方向に動かしてtrasnform_.quaRot.zを動かす
    int mx, my;
    GetMousePoint(&mx, &my);

    // 前フレームからの移動量の差分
    float deltaX = static_cast<float>(mx - lastMousePosX_);

    // マウスによるz軸のローカル回転
    transform_.quaRotLocal.x += -deltaX * MOUSE_SENSITIVITY;

    // ローカル回転の制限
    float thresholdRotZ = static_cast<float>(transform_.quaRotLocal.x);
    if (thresholdRotZ > MAX_ROT_Z)
    {
        thresholdRotZ = MAX_ROT_Z;
    }
    else if (thresholdRotZ < MIN_ROT_Z)
    {
        thresholdRotZ = MIN_ROT_Z;
    }

    // マウス座標の更新
    lastMousePosX_ = mx;

    // 判定の更新
    transform_.quaRotLocal.x= static_cast<double>(thresholdRotZ);
    currentAngle_ = static_cast<float>(transform_.quaRotLocal.x);

    // モデルの更新
    transform_.Update();
}

bool Wire::IsDifference(void)
{
    float diffAngle_ = std::abs(goalAngle_ - currentAngle_);
    if (diffAngle_ <= 10.0f)
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

bool Wire::GetDefault(void)
{
    return isDefault_;
}

void Wire::SetIsDefault(bool flag)
{
    isDefault_ = flag;
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

void Wire::SetDefault(void)
{
    // 座標
    auto camera = Application::GetInstance().GetCamera();
    // ① カメラの位置から
    VECTOR camPos = camera->GetPos();

    // ② カメラの「前方ベクトル」（GetForward）を計算し
    VECTOR forward = camera->GetForward();

    // ③ 前方に適切な距離（例: 300.0f）だけ進んだ地点をモデルの位置とする
    // この位置が、カメラの向きにかかわらず画面中央になります
    VECTOR modelPos = VAdd(camPos, VScale(forward, 220.0f));

    //transform_.pos = modelPos;
    transform_.pos = { 0.0f,0.0f ,-1000.0f };
    //transform_.pos.y += 60.0f;
    transform_.scl = { 0.0f,0.0f,0.0f };
    transform_.quaRot = Quaternion::LookRotation(camera->GetForward());

    // ローカル回転
    transform_.quaRotLocal = {
        Quaternion::Euler(
                AsoUtility::Deg2RadF(120.0f),
                AsoUtility::Deg2RadF(0.0f),
                AsoUtility::Deg2RadF(0.0f))
    };

    transform_.Update();

    // 一度だけ呼び出す
    isDefault_ = false;
}
