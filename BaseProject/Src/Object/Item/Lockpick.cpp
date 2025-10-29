#include <functional>
#include <algorithm>
#include <cmath>
#include "../../Application.h"
#include "../../Common/Quaternion.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include "Lockpick.h"

Lockpick::Lockpick(Player& player)
	:
	ItemBase(player),
	isUse_(false),
	angle_(0.0f),
	lLevel_(-1),
	rotRate_(0.0f),
	isDefault_(false),
	isUnlocking_(false),
	isSuccess_(false)
{
}

Lockpick::~Lockpick(void)
{
}

void Lockpick::Init(void)
{
	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::LOCKPICK));
	InitModel(
		INIT_POS,
		INIT_SCL,
		INIT_QUAROTLOCAL);

	angle_ = static_cast<float>(transform_.quaRotLocal.z);

	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	ChangeState(STATE::ININVENTORY);
}

void Lockpick::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void Lockpick::Draw(void)
{
	if (IsCurrentSelected() ||
		isUnlocking_)
	{
		MV1DrawModel(transform_.modelId);
		return;
	}
	DrawSphere3D(transform_.pos, 80.0f, 32, GetColor(255, 0, 0), GetColor(0, 255, 255), TRUE);

}

bool Lockpick::IsUse(bool use)
{
	isUse_ = use;
	return isUse_;
}

void Lockpick::OnUpdate(float deltaTime)
{
}

void Lockpick::UpdateOnStage(float deltaTime)
{
	// ステージにアイテムが落ちている状態
	// アイテムが拾われる処理
	

	//拾われるとUpdateInVentoryに遷移
	if (!isOnStage_)
	{
		ChangeState(STATE::ININVENTORY);
	}
}

void Lockpick::UpdateInVentory(float deltaTime)
{
	// 追従
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	// 現在手に持っているかどうかをスロットを見て判断する
	// 持っていれば

	transform_.scl = { 15.0f,15.0f,15.0f };

	// 装備しているかどうか
	if (isEquipment_)
	{
		ChangeState(STATE::INUSE);
	}
}

void Lockpick::UpdateInUse(float deltaTime)
{
	// プレイヤーに追尾
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	if (isUse_)
	{

	}
}

void Lockpick::UpdateUsedUp(float deltaTime)
{
	// アイテムが今後使用できなくなった場合

}

void Lockpick::UpdateDisabled(float deltaTime)
{
	// 一時的に使えない状態

	// 時間経過などで使える状態になったとき
	if (!isDisabled_)
	{
		// インベントリへ
		ChangeState(STATE::ININVENTORY);
	}

}

float Lockpick::CalculateRotRate(void)
{
	// 角度の差
	float diffAngle = angle_ - MIN_ANGLE;

	// 割合計算
	float ratio = diffAngle / TOTAL_RANGE;

	return std::clamp(ratio, 0.0f, 1.0f);
}

void Lockpick::SetDefault(void)
{
	// 座標
	auto camera = Application::GetInstance().GetCamera();
	// ① カメラの位置から
	VECTOR camPos = camera->GetPos();

	// ② カメラの「前方ベクトル」（GetForward）を計算し
	VECTOR forward = camera->GetForward();

	// ③ 前方に適切な距離（例: 300.0f）だけ進んだ地点をモデルの位置とする
	// この位置が、カメラの向きにかかわらず画面中央になります
	VECTOR modelPos = VAdd(camPos, VScale(forward, 200.0f));

	transform_.pos = modelPos;
	transform_.pos.y -= 60.0f;
	transform_.scl = { 100.0f,100.0f,100.0f };
	transform_.quaRot = Quaternion::LookRotation(camera->GetForward());

	// ローカル回転
	transform_.quaRotLocal = {
		Quaternion::Euler(
				AsoUtility::Deg2RadF(-90.0f),
				AsoUtility::Deg2RadF(0.0f),
				AsoUtility::Deg2RadF(90.0f))
	};

	transform_.Update();

	// 一度だけ呼び出す
	isDefault_ = false;
}

void Lockpick::UpdateUnlock(float deltaTime)
{
	if (isDefault_)
	{
		SetDefault();
	}

	// 回転割合
	if (isUnlocking_ && isSuccess_)
	{
		//// 目標角度を例えば -30度 (度からラジアンに変換)
		//const float GOAL_ROT = AsoUtility::Deg2RadF(-30.0f);
		//// 回転速度
		//const float ROT_SPEED = 5.0f * deltaTime; // 適当な値

		//// 現在のZ回転を取得
		//float currentRotZ = static_cast<float>(transform_.quaRotLocal.z);

		//// 目標に向かって徐々に回転
		//currentRotZ = AsoUtility::MoveToAngle(currentRotZ, GOAL_ROT, ROT_SPEED);

		//// 回転の適用
		//transform_.quaRotLocal.z = static_cast<double>(currentRotZ);

		//// 回転完了判定（回転がほぼ目標に達したら、次のシーン遷移などに備える）
		//if (std::abs(currentRotZ - GOAL_ROT) < 0.01f)
		//{
		//	// 解錠完了
		//	isUnlocking_ = false;
		//	// 次のシーンへの遷移ロジックをUnlickScene::NormalUpdateに記述
		//}
	}

	transform_.Update();
}

void Lockpick::SetLockLevel(int level)
{
	lLevel_ = level;
}

float Lockpick::GetRotationRate(void)
{
	return rotRate_;
}

bool Lockpick::GetDefault(void)
{
	return isDefault_;
}

void Lockpick::SetIsDefault(bool flag)
{
	isDefault_ = flag;
}

void Lockpick::SetIsUnlocking(bool flag)
{
	isUnlocking_ = flag;
}

void Lockpick::SetIsSuccess(bool flag)
{
	isSuccess_ = flag;
}


