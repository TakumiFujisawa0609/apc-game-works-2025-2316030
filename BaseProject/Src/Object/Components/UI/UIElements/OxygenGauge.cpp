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
	// 酸素値の割合に基づいてゲージが減る部分を計算
	float oxygenRatio = oxygen_.GetDepletionRate() / 100.0f;
	int rectHight = static_cast<int>(gaugeRadius_ * 2 * oxygenRatio);
	int rectY = static_cast<int>(gaugeCenterY_ + gaugeRadius_ - rectHight);

	// 塗りつぶす円(画像)の描画(シェーダ)を使って
	//テクスチャ座標＋v方向に向かって見えないようにする
	DrawCircle(gaugeCenterX_, gaugeCenterY_, gaugeRadius_,  GetColor(0, 250, 154), true);

	// 塗りつぶすされた円の上部を別の色で上書き
	// この円を酸素値の減少に合わせて描画することによって、上から減っていくように見せる
	int eraseHeight = static_cast<int>(gaugeRadius_ * 2 * (1.0f - oxygenRatio));
	int eraseY = static_cast<int>(gaugeCenterY_ - gaugeRadius_);

	// ゲージの外枠(リングを描画)
	//このリングは常に全体が表示される
	DrawCircle(gaugeCenterX_, gaugeCenterY_, gaugeRadius_, GetColor(0, 255, 255), false);

	// 酸素値のテキスト描画
	//----------------------------------------------------------------------------------------------
	wchar_t oxygenTextBuff[16];
	swprintf_s(oxygenTextBuff, L"%d", static_cast<int>(oxygen_.GetOxygen()));
	std::wstring oxygenText(oxygenTextBuff);

	// テキストをゲージ中央に配置するための計算
	int textWidth = GetDrawStringWidthToHandle(oxygenText.c_str(), wcslen(oxygenText.c_str()), Application::GetInstance().GetFontHandle());
	int textX = gaugeCenterX_ - textWidth / 2;

	// ワイド文字列描画関数を使用
	DrawStringToHandle(textX, gaugeCenterY_-12, oxygenText.c_str(), GetColor(0, 0, 0), Application::GetInstance().GetFontHandle());
	//----------------------------------------------------------------------------------------------
}
