#include "LightManager.h"

LightManager::LightManager(void)
	:
	playerPos_(nullptr)
{
	// HLSLで定義した構造体のサイズに合わせて定数バッファを作成
	constexpr int LIGHT_BUF_FLOAT4_SIZE = 18;

	// GPU側の定数バッファを作成し、ハンドルを取得
	lightConstBufH_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * LIGHT_BUF_FLOAT4_SIZE);

	// CPU側のデータを初期化
	lightConstBufs_.resize(LIGHT_BUF_FLOAT4_SIZE);
	std::memset(lightConstBufs_.data(), 0, sizeof(FLOAT4) * LIGHT_BUF_FLOAT4_SIZE);
	
	activeLightsNum_ = 0;
}

LightManager::~LightManager(void)
{
	DeleteShaderConstantBuffer(lightConstBufH_);
}

void LightManager::Init(void)
{
}

void LightManager::Update(void)
{
}

void LightManager::UpdatePlayerPointLight()
{
	// プレイヤー光源をリストの最初の要素として扱う


	

	// 
}

void LightManager::SetLightPos(const VECTOR* pos)
{
	playerPos_ = pos;
}
