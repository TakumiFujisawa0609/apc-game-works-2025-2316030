#include <algorithm>
#include <DxLib.h>
#include "../../../Application.h"
#include "OxygenComponent.h"

OxygenComponent::OxygenComponent(std::shared_ptr<ActorBase> owner,
	float max, float depletionRate)
    :
	Component(owner, L"OxygenComponent"),
	oxygen_(max),
	maxOxygen_(max),
	depletionRate_(depletionRate),
	isDepleted_(false)
{
}

OxygenComponent::~OxygenComponent()
{
}

void OxygenComponent::Update(float deltaTime)
{
	if (isDepleted_) return;

	// _‘f‚ªŒ¸­‚·‚éˆ—
	oxygen_ -= depletionRate_ * deltaTime;
	if (oxygen_ < 0.0f) {
		oxygen_ = 0.0f;
		isDepleted_ = true; // _‘fØ‚ê
	}

	int x = Application::SCREEN_SIZE_X / 6;
	int y = Application::SCREEN_SIZE_Y / 5;

	DrawFormatString(x, y, 0xffffff, L"oxygen=%f", oxygen_);
}

float OxygenComponent::GetOxygen() const
{
    return oxygen_;
}

void OxygenComponent::AddOxygen(float amount)
{
	if (isDepleted_) return;
	oxygen_ = std::clamp(oxygen_ + amount, 0.0f, maxOxygen_);
}

bool OxygenComponent::IsOxygenDepleted() const
{
	return isDepleted_;
}

void OxygenComponent::Consume(float deltaTime)
{
	oxygen_ -= depletionRate_ * deltaTime;
	if (oxygen_ < 0.0f)oxygen_ = 0.0f;
}

float OxygenComponent::GetDepletionRate(void) const
{
	return depletionRate_;
}


