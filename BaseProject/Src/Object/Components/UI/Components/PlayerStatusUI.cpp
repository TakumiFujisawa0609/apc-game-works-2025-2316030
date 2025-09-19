#include "../../../Player/Player.h"
#include "../UIElements/OxygenGauge.h"
#include "PlayerStatusUI.h"

PlayerStatusUI::PlayerStatusUI(std::shared_ptr<ActorBase> owner,Player& player)
	:
	UIComponent(owner),
	player_(player)
{
	// é_ëfÉQÅ[ÉW
	oxygen_ = AddElement<OxygenGauge>(*player_.GetOxygenComp());

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
