#define NOMINMAX
#include <algorithm>
#include <string>
#include <format>
#include <DxLib.h>
#include "../../../../Application.h"
#include "../../Gameplay/OxygenComponent.h"
#include "OxygenGauge.h"

OxygenGauge::OxygenGauge(OxygenComponent& oxygen)
	:
	oxygen_(oxygen)
{
	gaugeCenterX_ = static_cast<float>(Application::SCREEN_SIZE_X * 0.05);
	gaugeCenterY_ = static_cast<float>(Application::SCREEN_SIZE_Y * 0.68);
	gaugeRadius_ = std::min(gaugeCenterX_, gaugeCenterY_) * 0.5;

}

OxygenGauge::~OxygenGauge()
{
}

void OxygenGauge::Update(float deltaTime)
{

}

void OxygenGauge::Draw(void)
{
	// ÉQÅ[ÉWÇÃäOòg
	DrawCircleAA(gaugeCenterX_, gaugeCenterY_, 32.0f, 32.0f, GetColor(0, 250, 154), true);
	
	// écÇËé_ëféûä‘
	std::wstring text[256];
	//sprintf(text, "%.1f%%", oxygen_.GetOxygen());
	
}
