#include "../../../Player/Player.h"
#include "../UIElements/\TimeGauge.h"
#include "PlayerStatusUI.h"

PlayerStatusUI::PlayerStatusUI(std::shared_ptr<ActorBase> owner, Player& player)
	:
	UIComponent(owner),
	player_(player)
{
	// é_ëfÉQÅ[ÉW
	tGauge_ = AddElement<TimeGauge>(*player_.GetTimeLimitComp());

}

PlayerStatusUI::~PlayerStatusUI(void)
{
}

void PlayerStatusUI::Update(float deltaTime)
{
	UIComponent::Update(deltaTime);
}

void PlayerStatusUI::Draw(void)
{
	UIComponent::Draw();
}
