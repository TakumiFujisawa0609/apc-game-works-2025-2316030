#include <functional>
#include "../../../Application.h"
#include "../../../Manager/Config.h"
#include "../../../Common/Quaternion.h"
#include "../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Renderer/LightRenderer.h"
#include "../../../Renderer/DepthRenderer.h"
#include "HandLight.h"

HandLight::HandLight(Player& player)
	:
	PermanentItemBase(player),
	value_(0)
{
}

HandLight::~HandLight(void)
{
}

void HandLight::Init(void)
{

	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FLASHLIGHT));
	InitModel(
		INIT_POS,
		INIT_SCL,
		INIT_QUAROTLOCAL);

	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	isActive_ = false;

	canToggle_ = true;
	toggleTimer_ = 0.0f;

	value_ = MAX_VALUE;
	ChangeState(STATE::ONSTAGE);
	

	InitRenderer();
}

void HandLight::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	if (value_ / MAX_VALUE < 0.0f)value_ = 0.0f;

	//renderer_->UpdateRenderer(deltaTime, isActive_);
	//depthRenderer_->UpdateRenderer(deltaTime);
	
	// モデルの更新
	transform_.Update();
}

void HandLight::Draw(void)
{
	if (GetState() == STATE::ONSTAGE ||
		GetUse() != USE::NONE)
	{
		MV1DrawModel(transform_.modelId);

		//renderer_->DrawRenderer();
		//depthRenderer_->DrawRenderer();
		return;
	}
}

void HandLight::DrawUI(void)
{
	if (GetState() == STATE::ININVENTORY)
	{
		auto& size = Config::GetInstance().GetWindowSize();

		// --- UI描画の位置と設定 ---
		// 画面右下から配置を調整 (例: 右端から 250px、下端から 50px の位置)
		int uiWidth = static_cast<int>(size.width_ * 0.3125f);
		int uiHeight = static_cast<int>(size.width_ * 0.01f);

		// X座標: 画面幅 - 幅 - マージン
		int drawX = static_cast<int>(size.width_ - uiWidth - size.width_ * 0.078125f);
		// Y座標: 画面高さ - 高さ - マージン
		int drawY = static_cast<int>(size.height_ - uiHeight - size.width_ * 0.1875f); // アイテムスロットと被らないように配置を調整

		// ... ゲージの描画ロジックはそのまま利用 ...
		float ratio = value_ / MAX_VALUE;
		if (ratio < 0.0f) ratio = 0.0f;

		// 1. ゲージの背景を描画 (灰色)
		DrawBox(drawX, drawY, drawX + uiWidth, drawY + uiHeight, GetColor(50, 50, 50), TRUE);

		// 2. 残量ゲージを描画 (緑色 or 残量低下で赤色)
		int gaugeColor = (ratio > 0.3f) ? GetColor(0, 255, 0) : GetColor(255, 150, 0); // 30%以下で黄色に変化
		int fillWidth = (int)(uiWidth * ratio);
		DrawBox(drawX, drawY, drawX + fillWidth, drawY + uiHeight, gaugeColor, TRUE);

		// 3. テキスト表示
		int percent = (int)(ratio * 100.0f);
		DrawFormatString(drawX, static_cast<int>(drawY - size.height_ * 0.04167f), GetColor(255, 255, 255), L"LIGHT: %d%%", percent);
	}
}

void HandLight::ChangeBattery(float value)
{
	value_ = value;
}

float HandLight::GetRemainingPercentage(void)
{
	return value_ / MAX_VALUE;
}

int HandLight::GetRendererDepthScreen(void)
{
	//return renderer_->GetRendererDepthScreen();
	if (depthRenderer_ != nullptr) {
		return depthRenderer_->GetRendererDepthScreen();
	}
	return false;
}

void HandLight::OnUpdate(float deltaTime)
{
}

void HandLight::UpdateOnStage(float deltaTime)
{
	// ステージにアイテムが落ちている状態
	// アイテムが拾われる処理


	//拾われるとUpdateInVentoryに遷移
	if (!isOnStage_)
	{
		ChangeState(STATE::ININVENTORY);
	}
}

void HandLight::UpdateInVentory(float deltaTime)
{
	// 追従
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	// 装備しているかどうか

	ItemBase::UpdateUsed(deltaTime);
}

void HandLight::UpdateInUse(float deltaTime)
{

	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	auto& ins = InputManager::GetInstance();
	if (!canToggle_)
	{
		toggleTimer_ += deltaTime; // Application::GetInstance().GetDeltaTime() を使用
		if (toggleTimer_ >= TOGGLE_COOLDOWN)
		{
			canToggle_ = true;
			toggleTimer_ = 0.0f;
		}
	}
	if (canToggle_ && ins.IsClickMouseLeft())
	{
		isActive_ = !isActive_;

		// トグルを実行したらクールダウン開始
		canToggle_ = false;
		toggleTimer_ = 0.0f;
	}

	if (isActive_)
	{
		value_ -= Application::GetInstance().GetDeltaTime();

		if (value_ <= 0.0f)
		{
			value_ = 0.0f;
			isActive_ = false;
		}
	}

}

void HandLight::UpdateUsedUp(float deltaTime)
{
	//// アイテムが今後使用できなくなった場合
	//isDisabled_ = true;
}

void HandLight::InitRenderer(void)
{
	//renderer_ = std::make_unique<LightRenderer>();
	//renderer_->SetHandLight(this);
	//renderer_->InitLightRenderer(LightRenderer::TYPE::REGIDBODY, transform_.modelId);

	//depthRenderer_ = std::make_unique<DepthRenderer>();
	//depthRenderer_->InitLightRenderer(DepthRenderer::TYPE::REGIDBODY,transform_.modelId);

}