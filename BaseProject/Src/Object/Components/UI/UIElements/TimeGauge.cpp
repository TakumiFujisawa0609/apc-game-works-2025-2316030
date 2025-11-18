#define NOMINMAX
#include <algorithm>
#include <string>
#include <format>
#include <DxLib.h>
#include "../../../../Application.h"
#include "../../../../Manager/Config.h"
#include "../../Gameplay/OxygenComponent.h"
#include "TimeGauge.h"

TimeGauge::TimeGauge(TimeLimitComponent& tLimit)
	:
	tLimit_(tLimit)
{
	gaugeCenterX_ = static_cast<float>(Config::GetInstance().GetWindowSize().width_ * 0.05);
	gaugeCenterY_ = static_cast<float>(Config::GetInstance().GetWindowSize().height_ * 0.68);
	gaugeRadius_ = std::min(gaugeCenterX_, gaugeCenterY_) * 0.5f;

}

TimeGauge::~TimeGauge()
{
}

void TimeGauge::Update(float deltaTime)
{

}

void TimeGauge::Draw(void)
{
	// 酸素値の割合に基づいてゲージが減る部分を計算
	float oxygenRatio = tLimit_.GetDepletionRate() / 100.0f;
	int rectHight = static_cast<int>(gaugeRadius_ * 2 * oxygenRatio);
	int rectY = static_cast<int>(gaugeCenterY_ + gaugeRadius_ - rectHight);

	// 塗りつぶす円(画像)の描画(シェーダ)を使って
	//テクスチャ座標＋v方向に向かって見えないようにする
	DrawCircle(static_cast<int>(gaugeCenterX_), static_cast<int>(gaugeCenterY_), static_cast<int>(gaugeRadius_),  GetColor(0, 250, 154), true);

	// 塗りつぶすされた円の上部を別の色で上書き
	// この円を酸素値の減少に合わせて描画することによって、上から減っていくように見せる
	int eraseHeight = static_cast<int>(gaugeRadius_ * 2 * (1.0f - oxygenRatio));
	int eraseY = static_cast<int>(gaugeCenterY_ - gaugeRadius_);

	// ゲージの外枠(リングを描画)
	//このリングは常に全体が表示される
	DrawCircle(static_cast<int>(gaugeCenterX_), static_cast<int>(gaugeCenterY_), static_cast<int>(gaugeRadius_), GetColor(0, 255, 255), false);

	// 酸素値のテキスト描画
	//----------------------------------------------------------------------------------------------
	wchar_t oxygenTextBuff[16];
	swprintf_s(oxygenTextBuff, L"%d", static_cast<int>(tLimit_.GetTimeValue()));
	std::wstring oxygenText(oxygenTextBuff);

	// テキストをゲージ中央に配置するための計算
	int textWidth = GetDrawStringWidthToHandle(oxygenText.c_str(), static_cast<int>(wcslen(oxygenText.c_str())), static_cast<int>(Application::GetInstance().GetFontHandle()));
	int textX = static_cast<int>(gaugeCenterX_) - textWidth / 2;

	// ワイド文字列描画関数を使用
	DrawStringToHandle(textX, static_cast<int>(gaugeCenterY_ - 12), oxygenText.c_str(), GetColor(0, 0, 0), Application::GetInstance().GetFontHandle());
	//----------------------------------------------------------------------------------------------
}
