#include"../Object/Item/PermanentItems/HandLight.h"
#include "ModelMaterial.h"
#include "ModelRenderer.h"
#include "LightRenderer.h"

LightRenderer::LightRenderer(void)
{
}

LightRenderer::~LightRenderer(void)
{
}

void LightRenderer::InitLightRenderer(const TYPE& type, int modelId)
{
	float OutAngle = 0.5f;
	float InAngle = 0.15f;
	float Range = 500.0f;
	float Atten0 = 0.5f;
	float Atten1 = 0.0005f;
	float Atten2 = 0.001f;

	VECTOR pos = light_.lock()->GetTransform().pos;
	VECTOR dir = light_.lock()->GetTransform().quaRot.GetForward();

	ChangeLightTypeSpot(
		VGet(pos.x, pos.y, pos.z),
		VGet(dir.x, dir.y, dir.z),
		OutAngle,
		InAngle,
		Range,
		Atten0,
		Atten1,
		Atten2);

	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// ƒ‚ƒfƒ‹•`‰æ—p
	material_ = std::make_unique<ModelMaterial>(L"SpotLightAndPointLightVS.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
	if (type == TYPE::SKINING) {
		material_ = std::make_unique<ModelMaterial>(L"SkinMesh.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
	}
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	float currentTime = GetNowCount() / 1000.0f;
	float sin_value = (std::sin(5.0f * currentTime) + 1.0f) / 2.0f;
	blinkIntensity_ = 0.2f + (1.0f - 0.2f) * sin_value;
	material_->AddConstBufPS({ blinkIntensity_,0.0f,0.0f,0.0f });

	renderer_ = std::make_unique<ModelRenderer>(modelId, *material_);

}

void LightRenderer::UpdateRenderer(float deltaTime)
{
}

void LightRenderer::DrawRenderer(void)
{
}

int LightRenderer::GetRendererDepthScreen(void)
{
	return renderer_->GetDepthScreen();
}

void LightRenderer::SetHandLight(std::shared_ptr<HandLight> light)
{
}
