#include <DxLib.h>
#include <memory>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Collider.h"
#include "../Common/Capsule.h"
#include "Inventory.h"
#include "Player.h"
#include "../../Object/Components/Gameplay/OxygenComponent.h"
#include "../../Object/Components/Charactor/Player/PlayerInput.h"

Player::Player(void)
    :
    tLimit_(nullptr),
    input_(nullptr)
{
	
}

Player::~Player(void)
{
    DeleteSoundMem(walkSH_);
    DeleteSoundMem(runSH_);
}

void Player::Init(void)
{
	// モデル情報
	//transform_.pos = { -2000.0f, 200.0f, -1500.0f };
	//transform_.pos = { 500.0f, 200.0f, 800.0f };
	transform_.pos = { -500.0f, 600.0f, 100.0f };
	transform_.scl = { 1.0f, 1.0f, 1.0f };
    transform_.quaRot = Quaternion();
    transform_.quaRotLocal =
        Quaternion::Euler({ 0.0f,180.0f,0.0f });
	transform_.Update();

	SetMousePoint(Application::GetInstance().GetWindowSize().width / 2,
		Application::GetInstance().GetWindowSize().height / 2);

	moveSpeed_ = MOVE_WALK_SPEED; // 移動速度
    moveDir_ = AsoUtility::VECTOR_ZERO; // 移動方向
    movedPos_ = transform_.pos;

    startRotY_ = Quaternion();
    goalQuaRot_ = Quaternion();
    stepRotTime_ = 0.0f;

    yaw = 0.0f;       // 水平回転（ヨー）
	pitch = 0.0f;     // 垂直回転（ピッチ）

    // カプセルの初期化
    InitializeCapsule({ 0.0f, 180.0f, 0.0f }, { 0.0f, -20.0f, 0.0f }, 20.0f);

    walkSH_ = resMng_.Load(ResourceManager::SRC::WALK_SE).handleId_;
    ChangeVolumeSoundMem(255, walkSH_);
    isWalkSH_ = false;
    runSH_ = resMng_.Load(ResourceManager::SRC::RUN_SE).handleId_;
    ChangeVolumeSoundMem(255, runSH_);
    isRunSH_ = false;
}

void Player::InitComponents(void)
{
    // コンポーネントを生成してアタッチ
    tLimit_ = AddCharactorComponent<TimeLimitComponent>(60.0f, 1.0f);
    input_ = AddCharactorComponent<PlayerInput>();
}

void Player::Update(float deltaTime)
{
	Charactor::Update(deltaTime);

    tLimit_->GetTimeValue();
}

void Player::OnUpdate(float deltaTime)
{
    // カメラの角度を取得
    VECTOR angles = Application::GetInstance().GetCamera()->GetAngles();

    // Quaternion更新
    Quaternion qYaw = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles.y), AsoUtility::AXIS_Y);
    Quaternion qPitch = Quaternion::AngleAxis(AsoUtility::Deg2RadF(angles.x), AsoUtility::AXIS_X);

    // プレイヤーの回転をカメラと同期
    transform_.quaRot = qYaw.Mult(qPitch);

    // === 入力で移動方向を決定 ===

    // カメラ情報取得
    auto camera = Application::GetInstance().GetCamera();

    // 前方方向
    VECTOR forward = camera->GetForward();
    // 右方向
    VECTOR right = camera->GetRight();
    // 上方向
    VECTOR up = camera->GetUp();

    // 重力方向に垂直なカメラ角度(XZ平面)を取得
    Quaternion cameraRot = Application::GetInstance().GetCamera()->GetQuaRotOutX();

    // 回転したい角度
    double rotRad = 0.0;

    // 移動方向のリセット
    moveDir_ = AsoUtility::VECTOR_ZERO;

    // 入力関連
    auto& ins = InputManager::GetInstance();

    DINPUT_JOYSTATE pad;
    GetJoypadDirectInputState(DX_INPUT_PAD1, &pad);

    // 前後左右入力
    if (ins.IsNew(KEY_INPUT_W) || pad.Y < -500) moveDir_ = VAdd(moveDir_, forward);
    if (ins.IsNew(KEY_INPUT_S) || pad.Y > 500) moveDir_ = VAdd(moveDir_, VScale(forward, -1.0f));
    if (ins.IsNew(KEY_INPUT_D) || pad.X > 500) moveDir_ = VAdd(moveDir_, right);
    if (ins.IsNew(KEY_INPUT_A) || pad.X < -500) moveDir_ = VAdd(moveDir_, VScale(right, -1.0f));

    if (ins.IsNew(KEY_INPUT_LSHIFT) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
    {
        moveSpeed_ = MOVE_RUN_SPEED;
    }
    else
    {
        moveSpeed_ = MOVE_WALK_SPEED;
    }

    if (!AsoUtility::EqualsVZero(moveDir_))
    {
        moveDir_ = VNorm(moveDir_);
        rotRad = std::atan2(moveDir_.x, moveDir_.z);

        // 正規化して移動量を算出
        VECTOR direction = VNorm(transform_.quaRot.PosAxis(moveDir_));

        // 移動量
        movePow_ = VScale(direction, moveSpeed_);

        // 回転処理
        SetGoalRotate(static_cast<float>(rotRad));

        if (moveSpeed_ == MOVE_RUN_SPEED)
        {
            if (!isRunSH_)
            {
                StopSoundMem(walkSH_);
                PlaySoundMem(runSH_, DX_PLAYTYPE_LOOP, true);
                isRunSH_ = true;
            }
        }
        else
        {
            if (!isWalkSH_)
            {
                StopSoundMem(runSH_);
                PlaySoundMem(walkSH_, DX_PLAYTYPE_LOOP, true);
                isWalkSH_ = true;
            }
        }
    }
    else
    {
        movePow_ = AsoUtility::VECTOR_ZERO;

        StopSoundMem(runSH_);
        isRunSH_ = false;
        StopSoundMem(walkSH_);
        isWalkSH_ = false;
    }

    // 重力計算
    CalcGravityPow();

    Collision();

    // Transform更新
    transform_.Update();

}

void Player::Draw(void)
{

#ifdef _DEBUG

    //capsule_->Draw();
    //DrawFormatString(0, 20, GetColor(255, 255, 255), L"pos=(%.2f,%.2f,%.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);

    //DrawFormatString(0, 36, GetColor(255, 255, 255), L"quaRot=(%.2f,%.2f,%.2f)", transform_.quaRot.x, transform_.quaRot.y, transform_.quaRot.z);
#endif // _DEBUG

}

TimeLimitComponent* Player::GetTimeLimitComp()
{
	return tLimit_;
}

PlayerInput* Player::GetInputComp()
{
	return input_;
}

bool Player::TakeItem(int itemId, int count)
{
    return false;
}

int Player::GetWalkSH(void)
{
    return walkSH_;
}

int Player::GetRunSH(void)
{
    return runSH_;
}

void Player::SetGoalRotate(float rotRad)
{
    VECTOR angeles = Application::GetInstance().GetCamera()->GetAngles();
    double cameraRadY = AsoUtility::Deg2RadD(angeles.y);

    Quaternion axis = Quaternion::AngleAxis(cameraRadY + rotRad, AsoUtility::AXIS_Y);

    // 現在設定されている回転との角度差を取る
    double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

    // しきい値
    if (angleDiff > 0.1)
    {
        stepRotTime_ = 0.3f;
    }

    goalQuaRot_ = axis;
}

void Player::GiveItem(int itemId, int count)
{
	//inventory_->AddItem(itemId, count);
}

void Player::Collision(void)
{
    // 現在座標を起点に移動後座標を決める
    movedPos_ = VAdd(transform_.pos, movePow_);

    // 衝突(カプセル)
    CollisionCapsule();

    // 衝突(重力)
    CollisionGravity();

    // 移動
    transform_.pos = movedPos_;
}

void Player::CollisionCapsule(void)
{
    // カプセルを移動させる
    Transform trans = Transform(transform_);
    trans.pos = movedPos_;
    trans.Update();
    Capsule cap = Capsule(*capsule_, &trans);

    // カプセルとの衝突判定
    for (const auto& c : colliders_)
    {
        auto hits = MV1CollCheck_Capsule(
            c->modelId_, -1,
            cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

        for (int i = 0; i < hits.HitNum; i++)
        {
            auto hit = hits.Dim[i];
            for (int tryCnt = 0; tryCnt < 10; tryCnt++)
            {
                int pHit = HitCheck_Capsule_Triangle(
                    cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
                    hit.Position[0], hit.Position[1], hit.Position[2]);
                if (pHit)
                {
                    movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 0.10f));

                    // カプセルを移動させる
                    trans.pos = movedPos_;
                    trans.Update();
                    continue;
                }
                break;
            }
        }
        MV1CollResultPolyDimTerminate(hits);
    }
}

void Player::CollisionGravity(void)
{
    Charactor::CollisionGravity();
}

void Player::CalcGravityPow(void)
{
    Charactor::CalcGravityPow();
}
