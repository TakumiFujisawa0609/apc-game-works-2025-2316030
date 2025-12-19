#include "../../../Application.h"
#include "../../../Manager/Config.h"
#include "../../../Common/Quaternion.h"
#include "../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Utility/AsoUtility.h"
#include "../../Player/Player.h"
#include "Tranquilizer.h"

Tranquilizer::Tranquilizer(Player& player)
	:
	ConsumableItemBase(player)
{
}

Tranquilizer::~Tranquilizer(void)
{
}

void Tranquilizer::Init(void)
{

	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::TRANQUILIZER));
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

void Tranquilizer::Update(float deltaTime)
{
	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void Tranquilizer::Draw(void)
{
	if (GetState() == STATE::ONSTAGE ||
		GetUse() != USE::NONE){
		MV1DrawModel(transform_.modelId);
		auto& size = Config::GetInstance().GetWindowSize();
		return;
	}
}

void Tranquilizer::Use(void)
{
	isDisabled_ = true;
	player_.SetSanityLevel(SANITY_RECCVERY);
}

void Tranquilizer::OnUpdate(float deltaTime)
{
	ConsumableItemBase::UpdateOnStage(deltaTime);
}

void Tranquilizer::UpdateOnStage(float deltaTime)
{
}

void Tranquilizer::UpdateInVentory(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	ItemBase::UpdateInVentory(deltaTime);
}

void Tranquilizer::UpdateInUse(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);
}

void Tranquilizer::UpdateUsedUp(float deltaTime)
{
}