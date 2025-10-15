#include <cmath>
#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "Keyhole.h"

Keyhole::Keyhole(void)
{
}

Keyhole::~Keyhole(void)
{
}

void Keyhole::Init(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::KEYHOLE_I));

	auto camera = Application::GetInstance().GetCamera();
	// ① カメラの位置から
	VECTOR camPos = camera->GetPos();

	// ② カメラの「前方ベクトル」（GetForward）を計算し
	VECTOR forward = camera->GetForward();

	// ③ 前方に適切な距離（例: 300.0f）だけ進んだ地点をモデルの位置とする
	// この位置が、カメラの向きにかかわらず画面中央になります
	VECTOR modelPos = VAdd(camPos, VScale(forward, 200.0f));

	transform_.pos = modelPos;
	transform_.scl = { 66.0f,66.0f,66.0f };
	transform_.quaRot = Quaternion::LookRotation(camera->GetForward());;
	transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(90.0f),AsoUtility::Deg2RadF(0.0f) });

	isSuccess_ = false;

	// モデル情報の更新
	transform_.Update();
}

void Keyhole::Update(float deltaTime)
{
	//auto& ins = InputManager::GetInstance();

	//if (ins.IsTrgDown(KEY_INPUT_A))
	//{
	//	
	//}
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



	// モデルの更新
	transform_.Update();
}

void Keyhole::OnUpdate(float deltaTime)
{
}

void Keyhole::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	//DrawSphere3D(transform_.pos, 80.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);


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
