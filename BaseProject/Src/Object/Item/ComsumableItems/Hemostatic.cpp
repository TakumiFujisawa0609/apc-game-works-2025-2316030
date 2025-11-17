#include "../../../Manager/Config.h"
#include "../../../Common/Quaternion.h"
#include "../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Player/Player.h"
#include "Hemostatic.h"

Hemostatic::Hemostatic(Player& player)
	:
	ConsumableItemBase(player)
{
}

Hemostatic::~Hemostatic(void)
{
}

void Hemostatic::Init(void)
{

	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::HEMOSTATIC));
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

void Hemostatic::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void Hemostatic::Draw(void)
{
	if (GetState() == STATE::ONSTAGE ||
		GetUse() != USE::NONE)
	{
		MV1DrawModel(transform_.modelId);
		auto& size = Config::GetInstance().GetWindowSize();
		//DrawFormatString(size.width - 150, 144, GetColor(255, 255, 255), L"value = %.2f", value_);
		return;
	}
}

void Hemostatic::Use(void)
{
	isDisabled_ = true;
	player_.SetHitPoint(50.0f);
}

void Hemostatic::OnUpdate(float deltaTime)
{
	ConsumableItemBase::UpdateOnStage(deltaTime);

}

void Hemostatic::UpdateOnStage(float deltaTime)
{
}

void Hemostatic::UpdateInVentory(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	ItemBase::UpdateInVentory(deltaTime);
}

void Hemostatic::UpdateInUse(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);
}

void Hemostatic::UpdateUsedUp(float deltaTime)
{
}
