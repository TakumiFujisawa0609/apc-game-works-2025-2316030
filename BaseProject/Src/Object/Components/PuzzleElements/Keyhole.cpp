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
	if (isSuccess_)
	{
		//// 目標角度を例えば 90度 (度からラジアンに変換)
		//const float GOAL_ROT = AsoUtility::Deg2RadF(90.0f);
		//// 回転速度
		//const float ROT_SPEED = 5.0f * deltaTime; // Lockpickと同じか調整

		//float currentRotZ = static_cast<float>(transform_.quaRotLocal.z);

		//// 目標に向かって徐々に回転
		//currentRotZ = AsoUtility::MoveToAngle(currentRotZ, GOAL_ROT, ROT_SPEED);

		//// 回転の適用
		//transform_.quaRotLocal.z = static_cast<double>(currentRotZ);

		//// 回転完了判定 (鍵穴の回転完了が解錠完了と見なされることが多い)
		//if (std::abs(currentRotZ - GOAL_ROT) < 0.01f)
		//{
		//	// 解錠完了
		//	isSuccess_ = false;
		//	// UnlickSceneでシーン遷移をトリガーするフラグを立てる
		//}
	}
}

void Keyhole::Draw(void)
{
	PlazzleElementBase::Draw();
}

void Keyhole::SetAngle(float angle)
{
	transform_.quaRotLocal.z = angle;
}

void Keyhole::SetIsSuccess(bool isSuccess)
{
	isSuccess_ = isSuccess;
}

bool Keyhole::GetIsSuccess(void) const
{
	return isSuccess_;
}
