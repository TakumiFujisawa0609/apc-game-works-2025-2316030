#include <functional>
#include "../../../Application.h"
#include "../../../Common/Quaternion.h"
#include "../Manager/InputManager.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Renderer/ModelMaterial.h"
#include "../../../Renderer/ModelRenderer.h"
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

	canToggle_ = true;
	toggleTimer_ = 0.0f;

	blinkTimer_ = 0.0f;
	nextBlinkDuration_ = 0.0f;
	isBlinkActive_ = false;

	float randomValue = (float)GetRand(10000) / 10000.0f;
	nextBlinkDuration_ = BLINK_MIN_DURATION + (BLINK_MAX_DURATION - BLINK_MIN_DURATION) * randomValue;

	value_ = MAX_VALUE;
	ChangeState(STATE::ONSTAGE);

}

void HandLight::Update(float deltaTime)
{
	// モデル情報の動機

	// それぞれの状態での更新
	UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void HandLight::Draw(void)
{
	if (GetState() == STATE::ININVENTORY ||
		GetUse() != USE::INUSE)
	{
		MV1DrawModel(transform_.modelId);
		auto  size = Application::GetInstance().GetWindowSize();
		//DrawFormatString(size.width - 150, 144, GetColor(255, 255, 255), L"value = %.2f", value_);
		return;
	}
}

void HandLight::InitLightRenderer(const TYPE& type, int modelId)
{
	float OutAngle = 0.5f;
	float InAngle = 0.15f;
	float Range = 500.0f;
	float Atten0 = 0.5f;
	float Atten1 = 0.0005f;
	float Atten2 = 0.001f;

	VECTOR dir = transform_.quaRot.GetForward();

	ChangeLightTypeSpot(
		VGet(transform_.pos.x, transform_.pos.y, transform_.pos.z),
		VGet(dir.x, dir.y, dir.z),
		OutAngle,
		InAngle,
		Range,
		Atten0,
		Atten1,
		Atten2);

	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// モデル描画用
	material_ = std::make_unique<ModelMaterial>(L"SpotLightAndPointLightVS.cso", 2, L"SpotLightAndPointLightPS.cso", 3);
	material_->AddConstBufVS({ transform_.pos.x,transform_.pos.y,transform_.pos.z,0.0f });
	material_->AddConstBufVS({ dir.x + 0.1f,dir.y,dir.z,0.0f });
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	float currentTime = GetNowCount() / 1000.0f;
	float sin_value = (std::sin(5.0f * currentTime) + 1.0f) / 2.0f;
	blinkIntensity_ = 0.2f + (1.0f - 0.2f) * sin_value;
	material_->AddConstBufPS({ blinkIntensity_,0.0f,0.0f,0.0f });

	renderer_ = std::make_unique<ModelRenderer>(modelId, *material_);

}

void HandLight::UpdateRenderer(float deltaTime)
{
	float OutAngle = 0.5f;
	float InAngle = 0.15f;
	float Range = 1000.0f;
	float Atten0 = 0.5f;
	float Atten1 = 0.0005f;
	float Atten2 = 0.0f;

	VECTOR dir = transform_.quaRot.GetForward();

	ChangeLightTypeSpot(
		VGet(transform_.pos.x, transform_.pos.y, transform_.pos.z),
		VGet(dir.x, dir.y, dir.z),
		OutAngle,
		InAngle,
		Range,
		Atten0,
		Atten1,
		Atten2);

	material_->SetConstBufVS(0, { transform_.pos.x,transform_.pos.y,transform_.pos.z,0.0f });

	material_->SetConstBufVS(1, { dir.x + 0.1f,dir.y,dir.z,0.0f });

	float max = MAX_VALUE;
	float raito = value_ / max;
	if (raito < 0.0f)raito = 0.0f;
	if (isDisabled_)
	{
		if (raito <= 0.0f)
		{
			blinkIntensity_ = 0.0f;
		}
		else if (raito < 0.3f)
		{
			blinkTimer_ += deltaTime;
			if (blinkTimer_ >= nextBlinkDuration_)
			{
				// 時間が来たら状態を反転させる
				isBlinkActive_ = !isBlinkActive_;

				// 次の不規則な間隔を再設定
				float randomValue = (float)GetRand(10000) / 10000.0f;
				nextBlinkDuration_ = BLINK_MIN_DURATION + (BLINK_MAX_DURATION - BLINK_MIN_DURATION) * randomValue;
				blinkTimer_ = 0.0f;

			}
			if (isBlinkActive_)
			{
				blinkIntensity_ = 1.0f;
			}
			else
			{
				blinkIntensity_ = 0.2f;
			}
			float currentTime = GetNowCount() / 1000.0f;
			float sin_value = (std::sin(5.0f * currentTime) + 1.0f) / 2.0f;
			blinkIntensity_ = 0.2f + (1.0f - 0.2f) * sin_value;
		}
		else
		{
			blinkIntensity_ = 1.0f;
		}
	
		material_->SetConstBufPS(2, { blinkIntensity_,0.0f,0.0f,0.0f });
	}
	else
	{
		blinkIntensity_ = 0.0f;
		material_->SetConstBufPS(2, { blinkIntensity_,0.0f,0.0f,0.0f });
	}
}

void HandLight::DrawRenderer(void)
{
	renderer_->Draw();
}

void HandLight::DrawUI(void)
{

	/*if(GetUse()=USE::INUSE)
	{
		return;
	}*/

	// 描画設定を退避
	int blendMode;
	int blendParam;
	GetDrawBlendMode(&blendMode, &blendParam);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // UI描画前にブレンドモードをリセット

	auto size = Application::GetInstance().GetWindowSize();

	// --- UI描画の位置と設定 ---
	int drawX =  40; // 右から250ピクセルの位置
	int drawY = size.height - 50;  // 下から50ピクセルの位置
	int uiWidth = 200;
	int uiHeight = 20;

	//// value_が浮動小数点型であると仮定し、最大値で割って残量比率を計算
	float ratio = value_ / MAX_VALUE;
	if (ratio < 0.0f) ratio = 0.0f;

	// --- 1. ゲージの背景を描画 (灰色) ---
	DrawBox(drawX, drawY, drawX + uiWidth, drawY + uiHeight, GetColor(50, 50, 50), TRUE);

	// --- 2. 残量ゲージを描画 (緑色) ---
	// 残量に応じた幅
	int fillWidth = (int)(uiWidth * ratio);
	DrawBox(drawX, drawY, drawX + fillWidth, drawY + uiHeight, GetColor(0, 255, 0), TRUE);

	// --- 3. 残量パーセンテージをテキストで描画 ---
	int percent = (int)(ratio * 100.0f);
	DrawFormatString(drawX, drawY - 20, GetColor(255, 255, 255), L"残量: %d%%", percent);


	// SetDrawBlendMode(blendMode, blendParam);
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
		isDisabled_ = !isDisabled_;

		// トグルを実行したらクールダウン開始
		canToggle_ = false;
		toggleTimer_ = 0.0f;
	}

	if (isDisabled_)
	{
		value_ -= Application::GetInstance().GetDeltaTime();
	
		if (value_ <= 0.0f)
		{
			value_ = 0.0f;
			isDisabled_ = false;
		}
	}

}

void HandLight::UpdateUsedUp(float deltaTime)
{
	//// アイテムが今後使用できなくなった場合
	//isDisabled_ = true;
}