#include <functional>
#include "../../Application.h"
#include "../../Common/Quaternion.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "HandLight.h"

HandLight::HandLight(Player& player)
	:
	ItemBase(player),
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

	value_ = 100;
	ChangeState(STATE::ININVENTORY);

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

	if (IsCurrentSelected())
	{
		MV1DrawModel(transform_.modelId);
		return;
	}

	auto  size = Application::GetInstance().GetWindowSize();
	DrawFormatString(size.width - 150, 144, GetColor(255, 255, 255), L"value = %d", value_);

}

void HandLight::InitLightRenderer(TYPE type, int modelId)
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

	SetGlobalAmbientLight(GetColorF(0.8f, 0.8f, 0.8f, 0.8f));

	// モデル描画用
	material_ = std::make_unique<ModelMaterial>(L"SpotLightAndPointLightVS.cso", 2, L"SpotLightAndPointLightPS.cso", 2);
	material_->AddConstBufVS({ transform_.pos.x,transform_.pos.y,transform_.pos.z,0.0f });
	material_->AddConstBufVS({ dir.x,dir.y,dir.z,0.0f });
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });

	renderer_ = std::make_unique<ModelRenderer>(modelId, *material_);

}

void HandLight::UpdateRenderer()
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

	material_->SetConstBufVS(1, { dir.x,dir.y,dir.z,0.0f });

}

void HandLight::DrawRenderer()
{
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

	// 現在手に持っているかどうかをスロットを見て判断する
	// 持っていれば
	value_ -= Application::GetInstance().GetDeltaTime() / 100.0f;

	// 装備しているかどうか
	if (isEquipment_)
	{
		ChangeState(STATE::INUSE);
	}
}

void HandLight::UpdateInUse(float deltaTime)
{
	ItemBase::FollowTarget(deltaTime, TARGET_POS);

	// 現在手に持っているかどうかをスロットを見て判断する
	// 持っていれば
	value_ -= Application::GetInstance().GetDeltaTime() / 100.0f;

	if (value_ > 0)
	{
		ChangeState(STATE::USEDUP);
	}
}

void HandLight::UpdateUsedUp(float deltaTime)
{
	// アイテムが今後使用できなくなった場合
	isDisabled_ = true;
}

void HandLight::UpdateDisabled(float deltaTime)
{
	// 一時的に使えない状態

	// 時間経過などで使える状態になったとき
	if (!isDisabled_)
	{
		// インベントリへ
		ChangeState(STATE::ININVENTORY);
	}
}


