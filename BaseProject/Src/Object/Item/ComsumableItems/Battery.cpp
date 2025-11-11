#include "../../../Application.h"
#include "../../../Common/Quaternion.h"
#include "../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Utility/AsoUtility.h"
#include "../PermanentItems/HandLight.h"
#include "Battery.h"

Battery::Battery(Player& player)
	:
	ConsumableItemBase(player)
{
}

Battery::~Battery(void)
{
}

void Battery::Init(void)
{

	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BATTERY));
	InitModel(
		INIT_POS,
		INIT_SCL,
		INIT_QUAROTLOCAL);

	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;

	ChangeState(STATE::ONSTAGE);

	// モデルの更新
	transform_.Update();
}

void Battery::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void Battery::Draw(void)
{
	if (GetState() == STATE::ONSTAGE ||
		GetUse() != USE::NONE)
	{
		MV1DrawModel(transform_.modelId);
		return;
	}
	//DrawSphere3D(transform_.pos, 32, 16, GetColor(255, 0, 255), GetColor(255, 0, 255), false);
}

void Battery::Use(void)
{
	isDisabled_ = true;
	hLight_.lock()->ChangeBattery(100);
}

void Battery::SetHandLight(std::shared_ptr<HandLight> light)
{
	hLight_ = light;
}

void Battery::OnUpdate(float deltaTime)
{
	ConsumableItemBase::UpdateOnStage(deltaTime);
}

void Battery::UpdateInVentory(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	ItemBase::UpdateInVentory(deltaTime);
}

void Battery::UpdateInUse(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

}

void Battery::UpdateUsedUp(float deltaTime)
{
}

