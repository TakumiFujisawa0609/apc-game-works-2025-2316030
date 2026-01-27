#include <algorithm>
#include <DxLib.h>
#include "../../../Application.h"
#include "OxygenComponent.h"

TimeLimitComponent::TimeLimitComponent(std::shared_ptr<Charactor> owner,
	float max, float depletionRate)
    :
	CharactorComponent(owner),
	value_(max),
	maxValue_(max),
	depletionRate_(depletionRate),
	isDepleted_(false)
{
}

TimeLimitComponent::~TimeLimitComponent()
{
}

void TimeLimitComponent::Update(float deltaTime)
{
	if (isDepleted_) return;

	// ŠÔ‚ªŒ¸­‚·‚éˆ—
	value_ -= depletionRate_ * deltaTime;
	if (value_ < 0.0f) {
		value_ = 0.0f;

		// _‘fØ‚ê
		isDepleted_ = true;
	}

}

float TimeLimitComponent::GetTimeValue(void) const
{
    return value_;
}

void TimeLimitComponent::AddTimeValue(float amount)
{
	if (isDepleted_) return;
	value_ = std::clamp(value_ + amount, 0.0f, maxValue_);
}

bool TimeLimitComponent::IsTimeDepleted() const
{
	return isDepleted_;
}

void TimeLimitComponent::Consume(float deltaTime)
{
	value_ -= depletionRate_ * deltaTime;
	if (value_ < 0.0f)value_ = 0.0f;
}

float TimeLimitComponent::GetDepletionRate(void) const
{
	return depletionRate_;
}