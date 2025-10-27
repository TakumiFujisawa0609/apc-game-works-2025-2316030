#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>

class LightBase;

// 最大光源数(シェーダ―側の配列サイズと一致させる)
constexpr int MAX_LIGHTS = 4;
constexpr int POINT_LIGHT_BUF_SIZE = 3;	// FLOAT4 * 3
constexpr int SPOT_LIGHT_LIGHT_BUF_SIZE = 4;		// FLOAT4 * 4

class LightManager
{
public:

	LightManager(void);
	~LightManager(void);

	// 定数バッファを初期化、レジスタ番号を設定
	void Init(const int& shaderConstreg = 0);

	// フレーム毎に定数バッファを更新
	void Update(const VECTOR& viewPos);

	// 描画シェーダーへの定数バッファの設定
	void Draw(void);

	// ライトの追加と削除
	int AddLight(std::unique_ptr<LightBase> light);
	void RemoveLight(const int& lightId);

private:
	// シーン全体の光源情報 (HLSLのcbufferと構造を一致させる)
	struct LightContantBuffer
	{
		VECTOR viewPos_;		// 視点座標(スポットライトの座標)
		FLOAT4 pointLights_[MAX_LIGHTS * POINT_LIGHT_BUF_SIZE];
		FLOAT4 spotLights_[MAX_LIGHTS * SPOT_LIGHT_LIGHT_BUF_SIZE];
		FLOAT4 lightCount_;		// ポイント数、スポット数
	};

	int lightConstBufH_ = -1;	// 定数バッファハンドル
	LightContantBuffer lightConstBufs_;

	// ライトを一元管理するためのコンテナ
	std::vector<std::pair<int, std::unique_ptr<LightBase>>> lights_;
	int nextLightId_ = 0;		// 新しいライトに割り当てるID
	int shaderRegister_ = 0;	// シェーダー側の定数バッファレジスタ番号


};

