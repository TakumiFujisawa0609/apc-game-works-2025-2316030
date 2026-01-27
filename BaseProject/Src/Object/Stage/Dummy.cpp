#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Collider.h"
#include "../Common/Transform.h"
#include "Dummy.h"

Dummy::Dummy(void)
{
}

Dummy::~Dummy(void)
{
}

void Dummy::Init(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::DUMMY));
	transform_.scl = { 1.2f,1.2f,1.2f };
	transform_.pos = { 512.0f,12.0f,4100.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f),AsoUtility::Deg2RadF(0.0f) });;

	transform_.MakeCollider(Collider::TYPE::STAGE);

	transform_.Update();
}

void Dummy::Update(float deltaTime)
{
	transform_.Update();
}

void Dummy::OnUpdate(float deltaTime)
{
}

void Dummy::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}