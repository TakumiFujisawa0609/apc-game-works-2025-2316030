#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Collider.h"
#include "../Common/Transform.h"
#include "Stage.h"


Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::STAGE));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.Update();
}

void Stage::Update(float deltaTime)
{
}

void Stage::OnUpdate(float deltaTime)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}
