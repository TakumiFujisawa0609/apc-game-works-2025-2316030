#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Input.h"
#include "../Manager/SceneController.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Common/Capsule.h"

#include "Submarine.h"

Submarine::Submarine(void)
{
	stateMap_[STATE::NONE] = [this]() {UpdateNone();};
	stateMap_[STATE::MOVE] = [this]() {UpdateMove();};
	stateMap_[STATE::STOP] = [this]() {UpdateStop();};
	stateMap_[STATE::DESTROY] = [this]() {UpdateDestroy();};
}

Submarine::~Submarine(void)
{
}

void Submarine::Init(void)
{
	// モデル基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SUBMARINE));

	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRot = 
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });

	transform_.Update();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({});
	capsule_->SetLocalPosDown({});
	capsule_->SetRadius(0.0f);


	// 初期状態
	state_ = STATE::NONE;
}

void Submarine::Update(float deltaTime)
{
	// 状態に応じた更新
	if (stateMap_.count(state_))stateMap_[state_]();

	// モデル制御更新
	transform_.Update();
}

void Submarine::OnUpdate(float deltaTime)
{
}

void Submarine::Draw(void)
{

	MV1DrawModel(transform_.modelId);
}

void Submarine::SetGoalRotate(double rotRad)
{
}

void Submarine::Rotate(void)
{
}

void Submarine::Collision(void)
{
}

void Submarine::UpdateNone(void)
{
}

void Submarine::UpdateMove(void)
{
}

void Submarine::UpdateStop(void)
{
}

void Submarine::UpdateDestroy(void)
{
}
