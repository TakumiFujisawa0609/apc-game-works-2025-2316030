#include <algorithm>
#include "../Object/Light/PointLight.h"
#include "../Object/Light/SpotLight.h"
#include "LightManager.h"

LightManager::LightManager(void)
{
}

LightManager::~LightManager(void)
{
	if (lightConstBufH_ != -1)
	{
		DeleteShaderConstantBuffer(lightConstBufH_);
	}
}

void LightManager::Init(const int& shaderConstreg)
{
	// GPU側のバッファを作成
	// FLOAT4の配列として扱うため、FLOAT4の総数*sizeof(FLOAT4)でサイズを決定する
	lightConstBufH_ = CreateShaderConstantBuffer(sizeof(lightConstBufs_));

	// データの初期化
	std::memset(&lightConstBufs_, 0, sizeof(LightContantBuffer));

	shaderRegister_ = shaderConstreg;
}

void LightManager::Update(const VECTOR& viewPos)
{
    // 1. ビュー位置の更新
    lightConstBufs_.viewPos_ = { viewPos.x, viewPos.y, viewPos.z};

    int pointCount = 0;
    int spotCount = 0;

    //// 2. ライトデータの抽出と定数バッファへの格納
    //for (const auto& pair : lights_) {
    //    const auto& light = pair.second;

    //    if (light->GetType() == LightBase::POINT && pointCount < MAX_LIGHTS) {
    //        const PointLight* pLight = static_cast<const PointLight*>(light.get());
    //        // PointLightのデータをコピー (FLOAT4 * 3分)
    //        const FLOAT4* src = reinterpret_cast<const FLOAT4*>(pLight->GetData());
    //        FLOAT4* dest = &lightConstBufs_.pointLights_[pointCount * POINT_LIGHT_BUF_SIZE];
    //        std::copy(src, src + POINT_LIGHT_BUF_SIZE, dest);
    //        pointCount++;

    //    }
    //    else if (light->GetType() == LightBase::SPOT && spotCount < MAX_LIGHTS) {
    //        const SpotLight* sLight = static_cast<const SpotLight*>(light.get());
    //        // SpotLightのデータをコピー (FLOAT4 * 4分)
    //        const FLOAT4* src = reinterpret_cast<const FLOAT4*>(sLight->GetData());
    //        FLOAT4* dest = &lightConstBufs_.spotLights_[spotCount * SPOT_LIGHT_BUF_SIZE];
    //        std::copy(src, src + SPOT_LIGHT_BUF_SIZE, dest);
    //        spotCount++;
    //    }

    //    // 個々のライトのUpdateを呼び出す (アニメーションなどのため)
    //    light->Update();
    //}

    //// 3. ライト数の更新
    //lightConstBufs_.lightCount_[0] = (float)pointCount;
    //lightConstBufs_.lightCount_[1] = (float)spotCount;

    //// 4. GPU定数バッファの更新
    //UpdateShaderConstantBuffer(lightConstBufH_, &lightConstBufs_);
}

void LightManager::Draw(void)
{
    //SetShaderConstantBuffer(shaderRegister_, lightConstBufH_);
}

int LightManager::AddLight(std::unique_ptr<LightBase> light)
{
    int id = nextLightId_++;
    lights_.push_back({ id, std::move(light) });
    return id;
}

void LightManager::RemoveLight(const int& lightId)
{
    lights_.erase(
        std::remove_if(lights_.begin(), lights_.end(),
            [lightId](const auto& pair) { return pair.first == lightId; }),
        lights_.end());
}
