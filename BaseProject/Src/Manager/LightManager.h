#pragma once
#include <vector>
#include <DxLib.h>

class LightManager
{
public:

	// ポイントライトのデータ構造
	struct PointLight
	{
		VECTOR pointWorldPos_;	// ワールド座標系
		float padding1_;

		VECTOR color_;		// 色
		float intensity_;	// 強度

		float radius_;		//影響範囲
		VECTOR paddind2_;
	};

	//　スポットライトのデータ構造
	struct SpotLight
	{
		VECTOR spotWorldPos_;	// ワールド座標系
		float padding3_;

		VECTOR dir_;			// 向き
		float padding4_;

		float outAngle_;		// スポットライトコーンの外側の角度
		float inAngle_;			// スポットライトコーンの内側の角度
		float range_;			// ライトの有効距離
		float padding5_;

		float atten0_;			// 距離減衰パラメータ０
		float atten1_;			// 距離減衰パラメータ１
		float atten2_;			// 距離減衰パラメータ２
		float padding6_;
	};

	struct Light
	{
		PointLight pointLight_;
		SpotLight spotLight_;
	};

	// シーン全体の光源情報
	struct LightContantBuffer
	{
		VECTOR viewPos_;
		float padding7_;

		Light lights[4];

		int numLights_;
		VECTOR padding8_;
	};

	LightManager(void);
	~LightManager(void);

	void Init(void);
	void Update(void);

	// プレイヤーに常に追尾するポイントライト
	void UpdatePlayerPointLight();


	void SetLightPos(const VECTOR* pos);

private:
	
	int lightConstBufH_;					// 定数バッファハンドル
	std::vector<FLOAT4> lightConstBufs_;	// 配置データ
	int activeLightsNum_;					// 使用可能光源数

	//　プレイヤー位置
	const VECTOR* playerPos_;

};

