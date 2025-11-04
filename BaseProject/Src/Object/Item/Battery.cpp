#include "../../Application.h"
#include "../../Common/Quaternion.h"
#include "../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include "Battery.h"

Battery::Battery(Player& player)
	:
	ItemBase(player)
{
}

Battery::~Battery(void)
{
}

void Battery::Init(void)
{
}

void Battery::Update(float deltaTime)
{

	transform_.Update();
}

void Battery::Draw(void)
{
}

void Battery::OnUpdate(float deltaTime)
{
}

void Battery::UpdateOnStage(float deltaTime)
{
}

void Battery::UpdateInVentory(float deltaTime)
{
}

void Battery::UpdateInUse(float deltaTime)
{
}

void Battery::UpdateUsedUp(float deltaTime)
{
}

void Battery::UpdateDisabled(float deltaTime)
{
}
