// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"


SamplerState shadowMap0Sampler            : register(s1);	    	// シャドウマップ０テクスチャ
Texture2D    shadowMap0Texture            : register(t1);

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_diff_color;
	float4 g_ambient_color;
	float g_blink_intensity;
	float3 g_fog_color;
}

struct LightCalculationData {
	float4 TextureDiffuseColor;		// テクスチャの拡散反射の色
	float4 SpecularDiffuseColor;	// テクスチャの鏡面反射の色
	float3 Normal;				// モデルの法線
	float DiffuseAngleGen;		// 拡散反射角度減衰率
	float4 TotalDiffuse;		// 最終的な拡散反射
	float4 TotalSpecular;		// 最終的な鏡面反射
	float3 V_to_Eye;			// モデル頂点から視線までのベクトル
	float3 TempF3;				//
	float Temp;
	float3 lLightTemp;
	float lLightDistancePow2;
	float lLightGen;
	float3 lLightDir;
	float lLightDirectionCosA;
};

float4 main(PS_INPUT PSInput) : SV_TARGET0
{

	float4 color;

	// テクスチャの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	if (color.a < 0.01f)
	{
		discard;
	}

	//return color;

	// マテリアルの情報を取得
	Material material = g_common.material;

	//PS_OUTPUT ret;

	// 一つ目のライト(スポットライト)の処理
	// -------------------------------------------
	Light spotL = g_common.light[0];

	// ライトの位置
	float3 spotLPos = spotL.position;

	// ライトの向き
	float3 spotLDir = spotL.direction;

	LightCalculationData lightData;

	//法線の準備
	lightData.Normal = normalize(PSInput.normal);

	//頂点座標から視点へのベクトルを正規化
	lightData.V_to_Eye = normalize(-PSInput.vwPos);
	//V_to_Eye = normalize(-PSInput.worldPos);

	//ディフューズカラーとスペキュラーカラーの蓄積値を初期化
	lightData.TotalDiffuse = g_diff_color;	 //(初期色)
	lightData.TotalSpecular = g_ambient_color; //(初期色)

	//スポットライトの処理+++++++++++++++++++++++++++++++++(開始)

	//ライト方向ベクトルの計算
	lightData.lLightTemp = PSInput.vwPos.xyz - spotLPos.xyz;
	lightData.lLightDir = normalize(lightData.lLightTemp);

	//距離・スポットライト減衰値計算-------------(開始)

	//距離減衰計算------------------

	//頂点とライト位置との距離の二乗を求める
	//float3 lsLightTemp = spotLPos.xyz - PSInput.worldPos.xyz;
	float3 lsLightTemp = spotLPos.xyz - PSInput.vwPos.xyz;
	lsLightTemp = PSInput.vwPos.xyz - spotLPos.xyz;
	lightData.lLightDistancePow2 = dot(lsLightTemp, lsLightTemp);

	//減衰率の計算　lLightGen = 1 / （減衰値0 + 減衰値1 * ( 距離 * 距離) ）
	lightData.lLightGen = 1.0f / (spotL.attenuation0 + spotL.attenuation1 * sqrt(lightData.lLightDistancePow2) + spotL.attenuation2 * lightData.lLightDistancePow2);

	//return float4(lLightGen,lLightGen,lLightGen,1.0f);
	//------------------------------

	//スポットライト減衰計算----------------------(開始)

	//ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積(即ち Cos a)を計算
	//float3 LtoV = normalize(spotLPos.xyz - PSInput.worldPos.xyz);
	float3 LtoV = normalize(spotLPos.xyz - PSInput.vwPos.xyz);
	lightData.lLightDirectionCosA = dot(lightData.lLightDir, (spotL.direction));

	// 一時的なデバッグ: lLightDirectionCosA だけを返す
	//return float4(lLightDirectionCosA, lLightDirectionCosA, lLightDirectionCosA, 1.0f);

	//スポットライト減衰計算　pow( falloff, ((Cos a - Cos f) / (Cos q - Cos f)))
	lightData.lLightGen *= saturate(pow(abs(max(lightData.lLightDirectionCosA - spotL.spotParam0, 0.0f) * spotL.spotParam1), spotL.fallOff));

	//最終的なスポットライトの減衰率を返す
	//return float4(lLightGen,lLightGen,lLightGen,1.0f);

	//------------------------------

	//有効距離外だったら減衰率を最大にする処理
	lightData.lLightGen *= step(lightData.lLightDistancePow2, spotL.rangePow2);

	// 点滅強度を乗算する
	lightData.lLightGen *= g_blink_intensity;

	//return float4(lLightGen, lLightGen, lLightGen, 1.0f);

	//距離・スポットライト減衰計算----------------(終了)

	//シャドウマップの処理

	//------------------------------------------

	//ディフューズ色計算--------------------------(開始)

	//ディフューズ角度減衰率計算
	lightData.DiffuseAngleGen = saturate(dot(lightData.Normal, -lightData.lLightDir));

	//ライト方向(ワールド座標系)
	//float3 WorldLightDir = normalize(spotL.position.xyz - PSInput.worldPos.xyz);
	float3 WorldLightDir = normalize(spotL.position.xyz - PSInput.vwPos.xyz);

	// ディフューズ角度減衰率計算
	float dot_N_L_original = dot(lightData.Normal, WorldLightDir);
	float dot_N_L_inverted = dot(-lightData.Normal, WorldLightDir); // 法線を反転して計算

	// どちらの内積が大きいか（つまり、どちらが光をよく受けているか）を比較
	lightData.DiffuseAngleGen = saturate(max(dot_N_L_original, dot_N_L_inverted));

	// 標準的なライティングの寄与
	lightData.TotalDiffuse += (float4(spotL.diffuse.xyz, 1.0f) * material.diffuse * normalize(lightData.DiffuseAngleGen)
	+ spotL.ambient) * lightData.lLightGen;

	//return float4(TotalDiffuse.x, TotalDiffuse.y, TotalDiffuse.z, TotalDiffuse.a);

	//スペキュラーカラー計算

	//ハーフベクトルの計算
	lightData.TempF3 = normalize(lightData.V_to_Eye - lightData.lLightDir);
	lightData.Temp = pow(max(0.0f, dot(lightData.Normal, lightData.TempF3)), material.power.x);

	//スペキュラーカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラーカラー
	lightData.TotalSpecular += lightData.Temp * lightData.lLightGen.x * (float4(spotL.specular.xyz, 1.0f));

	//スポットライトの処理+++++++++++++++++++++++++++++++++(終了)

	//出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(開始)

	//TotalDiffuse = ライトディフーズカラー蓄積値 + (マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの)
	lightData.TotalDiffuse += material.ambientEmissive;

	// 1. テクスチャカラーをサンプリング
	lightData.TextureDiffuseColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv.xy);

	// g_ambient_color をグローバルアンビエントとして、マテリアルの ambient 反射率を考慮して加算。
	lightData.TotalDiffuse.rgb += g_ambient_color.rgb * lightData.TextureDiffuseColor.rgb * g_diff_color.rgb;

	// SpecularColor = ライトのスペキュラーカラー蓄積値 * マテリアルのスペキュラーカラー
	lightData.SpecularDiffuseColor = lightData.TotalSpecular * material.specular;

	// 出力カラー計算 = TotalDiffuse * テクスチャカラー + SpecularColor
	float4 retColor;

	lightData.TextureDiffuseColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv.xy);
	retColor.rgb = lightData.TextureDiffuseColor.rgb * lightData.TotalDiffuse.rgb + lightData.SpecularDiffuseColor.rgb;

	//アルファ値 = テクスチャアルファ * マテリアルのディフューズアルファ * 不透明度
	retColor.a = lightData.TextureDiffuseColor.a * material.diffuse.a * g_base.factorColor.a;

	//出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(終了)

	//return retColor;


	// フォグ計算
	float fog_amount = 1.0f - PSInput.fogFactor;
	retColor.rgb = lerp(g_fog_color, retColor.rgb, fog_amount);

	retColor.rgb *= PSInput.fogFactor;

	//出力パラメータを返す
	return retColor;
}