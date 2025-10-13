#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "KeyholePlate.h"

KeyholePlate::KeyholePlate(void)
{

}

KeyholePlate::~KeyholePlate(void)
{
}

void KeyholePlate::Init(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::E_PLATE_I));

	auto camera = Application::GetInstance().GetCamera();
	// ① カメラの位置から
	VECTOR camPos = camera->GetPos();

	// ② カメラの「前方ベクトル」（GetForward）を計算し
	VECTOR forward = camera->GetForward();

	// ③ 前方に適切な距離（例: 300.0f）だけ進んだ地点をモデルの位置とする
	// この位置が、カメラの向きにかかわらず画面中央になります
	VECTOR modelPos = VAdd(camPos, VScale(forward, 300.0f));

	transform_.pos = modelPos;
	transform_.scl = { 200.0f,200.0f,200.0f };
	transform_.quaRot = Quaternion::LookRotation(camera->GetForward());
	transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(90.0f),AsoUtility::Deg2RadF(0.0f) });

	// モデル情報の更新
	transform_.Update();
}

void KeyholePlate::Update(float deltaTime)
{
	auto camera = Application::GetInstance().GetCamera();
	transform_.quaRot = camera->GetQuaRot();

	// モデルの更新
	transform_.Update();
}

void KeyholePlate::OnUpdate(float deltaTime)
{
}

void KeyholePlate::Draw(void)
{

	MV1DrawModel(transform_.modelId);
	//DrawSphere3D(transform_.pos, 80.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
}
