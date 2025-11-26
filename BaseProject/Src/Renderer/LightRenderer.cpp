<<<<<<< HEAD
#include "../Object/Item/PermanentItems/HandLight.h"
=======
#include"../Object/Item/PermanentItems/HandLight.h"
>>>>>>> ecefbc80def25fa1d0a042bdaabdac08dc61600b
#include "ModelMaterial.h"
#include "ModelRenderer.h"
#include "LightRenderer.h"

LightRenderer::LightRenderer(void)
<<<<<<< HEAD
    :
    blinkIntensity_(0.0f),
    blinkTimer_(0.0f),
    isBlinkActive_(false),
    nextBlinkDuration_(0.0f)
=======
	:
	blinkIntensity_(0.0f),
	blinkTimer_(0.0f),
	isBlinkActive_(false),
	nextBlinkDuration_(0.0f)
>>>>>>> ecefbc80def25fa1d0a042bdaabdac08dc61600b
{
}

LightRenderer::~LightRenderer(void)
{
}

void LightRenderer::InitLightRenderer(const TYPE& type, int modelId)
{

<<<<<<< HEAD
    float OutAngle = 0.5f;
    float InAngle = 0.15f;
    float Range = 500.0f;
    float Atten0 = 0.5f;
    float Atten1 = 0.0005f;
    float Atten2 = 0.001f;

    VECTOR pos = light_->GetTransform().pos;
    VECTOR dir = light_->GetTransform().quaRot.GetForward();

    ChangeLightTypeSpot(
        VGet(pos.x, pos.y, pos.z),
        VGet(dir.x, dir.y, dir.z),
        OUT_ANGLE,
        IN_ANGLE,
        RANGE,
        ATTEN0,
        ATTEN1,
        ATTEN2);

        SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));

        material_ = std::make_unique<ModelMaterial>(L"SpotLightAndPointLightVS.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
        material_ = std::make_unique<ModelMaterial>(L"NormalMeshSpotLightVS.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
        if (type == TYPE::SKINING) {
            material_ = std::make_unique<ModelMaterial>(L"SkinMesh.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
            material_ = std::make_unique<ModelMaterial>(L"SkinMeshSpotLightVS.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
        }
        material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
        material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
}

void LightRenderer::UpdateRenderer(float deltaTime)
{
=======
	VECTOR pos = light_->GetTransform().pos;
	VECTOR dir = light_->GetTransform().quaRot.GetForward();

	ChangeLightTypeSpot(
		VGet(pos.x, pos.y, pos.z),
		VGet(dir.x, dir.y, dir.z),
		OUT_ANGLE,
		IN_ANGLE,
		RANGE,
		ATTEN0,
		ATTEN1,
		ATTEN2);

	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// ƒ‚ƒfƒ‹•`‰æ—p
	material_ = std::make_unique<ModelMaterial>(L"NormalMeshSpotLightVS.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
	if (type == TYPE::SKINING) {
		material_ = std::make_unique<ModelMaterial>(L"SkinMeshSpotLightVS.cso", 0, L"SpotLightAndPointLightPS.cso", 3);
	}
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	float currentTime = GetNowCount() / 1000.0f;
	float sin_value = (std::sin(5.0f * currentTime) + 1.0f) / 2.0f;
	blinkIntensity_ = 0.2f + (1.0f - 0.2f) * sin_value;
	material_->AddConstBufPS({ blinkIntensity_,0.0f,0.0f,0.0f });

	renderer_ = std::make_unique<ModelRenderer>(modelId, *material_);

>>>>>>> ecefbc80def25fa1d0a042bdaabdac08dc61600b
}

void LightRenderer::UpdateRenderer(float deltaTime, bool isActive)
{
<<<<<<< HEAD
    VECTOR pos = light_->GetTransform().pos;
    VECTOR dir = light_->GetTransform().quaRot.GetForward();

    ChangeLightTypeSpot(
        VGet(pos.x, pos.y, pos.z),
        VGet(dir.x, dir.y, dir.z),
        OUT_ANGLE,
        IN_ANGLE,
        RANGE,
        ATTEN0,
        ATTEN1,
        ATTEN2);

    material_->SetConstBufVS(0, { pos.x,pos.y,pos.z,0.0f });

    material_->SetConstBufVS(1, { dir.x + 0.1f,dir.y,dir.z,0.0f });

    float raito = light_->GetRemainingPercentage();
    if (isActive) {
        if (raito <= 0.0f) {
            blinkIntensity_ = 0.0f;
        }
        else if (raito < 0.3f) {
            blinkTimer_ += deltaTime;
            if (blinkTimer_ >= nextBlinkDuration_) {
                isBlinkActive_ = !isBlinkActive_;

                float randomValue = (float)GetRand(10000) / 10000.0f;
                nextBlinkDuration_ = BLINK_MIN_DURATION + (BLINK_MAX_DURATION - BLINK_MIN_DURATION) * randomValue;
                blinkTimer_ = 0.0f;

            }
            if (isBlinkActive_) {
                blinkIntensity_ = 1.0f;
            }
            else {
                blinkIntensity_ = 0.2f;
            }
            float currentTime = GetNowCount() / 1000.0f;
            float sin_value = (std::sin(5.0f * currentTime) + 1.0f) / 2.0f;
            blinkIntensity_ = 0.2f + (1.0f - 0.2f) * sin_value;
        }
        else {
            blinkIntensity_ = 1.0f;
        }
        material_->SetConstBufPS(2, { blinkIntensity_,0.0f,0.0f,0.0f });
    }
    else {
        blinkIntensity_ = 0.0f;
        material_->SetConstBufPS(2, { blinkIntensity_,0.0f,0.0f,0.0f });
    }
=======
	VECTOR pos = light_->GetTransform().pos;
	VECTOR dir = light_->GetTransform().quaRot.GetForward();

	ChangeLightTypeSpot(
		VGet(pos.x, pos.y, pos.z),
		VGet(dir.x, dir.y, dir.z),
		OUT_ANGLE,
		IN_ANGLE,
		RANGE,
		ATTEN0,
		ATTEN1,
		ATTEN2);

	material_->SetConstBufVS(0, { pos.x,pos.y,pos.z,0.0f });

	material_->SetConstBufVS(1, { dir.x + 0.1f,dir.y,dir.z,0.0f });

	float raito = light_->GetRemainingPercentage();
	if (isActive) {
		if (raito <= 0.0f) {
			blinkIntensity_ = 0.0f;
		}
		else if (raito < 0.3f) {
			blinkTimer_ += deltaTime;
			if (blinkTimer_ >= nextBlinkDuration_) {
				// ŽžŠÔ‚ª—ˆ‚½‚çó‘Ô‚ð”½“]‚³‚¹‚é
				isBlinkActive_ = !isBlinkActive_;

				// ŽŸ‚Ì•s‹K‘¥‚ÈŠÔŠu‚ðÄÝ’è
				float randomValue = (float)GetRand(10000) / 10000.0f;
				nextBlinkDuration_ = BLINK_MIN_DURATION + (BLINK_MAX_DURATION - BLINK_MIN_DURATION) * randomValue;
				blinkTimer_ = 0.0f;

			}
			if (isBlinkActive_) {
				blinkIntensity_ = 1.0f;
			}
			else {
				blinkIntensity_ = 0.2f;
			}
			float currentTime = GetNowCount() / 1000.0f;
			float sin_value = (std::sin(5.0f * currentTime) + 1.0f) / 2.0f;
			blinkIntensity_ = 0.2f + (1.0f - 0.2f) * sin_value;
		}
		else {
			blinkIntensity_ = 1.0f;
		}
		material_->SetConstBufPS(2, { blinkIntensity_,0.0f,0.0f,0.0f });
	}
	else {
		blinkIntensity_ = 0.0f;
		material_->SetConstBufPS(2, { blinkIntensity_,0.0f,0.0f,0.0f });
	}
>>>>>>> ecefbc80def25fa1d0a042bdaabdac08dc61600b
}

void LightRenderer::DrawRenderer(void)
{
<<<<<<< HEAD
    renderer_->Draw();
}

void LightRenderer::SetHandLight(HandLight* light)
{
    light_ = light;
=======
	renderer_->Draw();
>>>>>>> ecefbc80def25fa1d0a042bdaabdac08dc61600b
}

int LightRenderer::GetRendererDepthScreen(void)
{
<<<<<<< HEAD
	return false;
=======
	return renderer_->GetDepthScreen();
}

void LightRenderer::SetHandLight(HandLight* light)
{
	light_ = light;
>>>>>>> ecefbc80def25fa1d0a042bdaabdac08dc61600b
}
