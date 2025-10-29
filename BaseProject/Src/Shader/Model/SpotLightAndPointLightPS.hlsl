// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_diff_color;
	float4 g_ambient_color;
}

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

	float3 spotLPos = spotL.position;
	float3 spotLDir = spotL.direction;

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

	//法線の準備
	Normal = normalize(PSInput.normal);

	//頂点座標から視点へのベクトルを正規化
	//V_to_Eye = normalize(-PSInput.vwPos);
	V_to_Eye = normalize(-PSInput.worldPos);

	//ディフューズカラーとスペキュラーカラーの蓄積値を初期化
	TotalDiffuse = g_diff_color;	 //(初期色)
	TotalSpecular = g_ambient_color; //(初期色)

	//スポットライトの処理+++++++++++++++++++++++++++++++++(開始)

	//ライト方向ベクトルの計算
	lLightTemp = PSInput.vwPos.xyz - spotLPos.xyz;
	lLightDir = normalize(lLightTemp);

	//距離・スポットライト減衰値計算-------------(開始)

	//距離減衰計算------------------

	//頂点とライト位置との距離の二乗を求める
	float3 lsLightTemp = spotLPos.xyz - PSInput.worldPos.xyz;
	lsLightTemp = PSInput.vwPos.xyz - spotLPos.xyz;
	lLightDistancePow2 = dot(lsLightTemp, lsLightTemp);

	//減衰率の計算　lLightGen = 1 / （減衰値0 + 減衰値1 * ( 距離 * 距離) ）
	lLightGen = 1.0f / (spotL.attenuation0 + spotL.attenuation1 * sqrt(lLightDistancePow2) + spotL.attenuation2 * lLightDistancePow2);

	//return float4(lLightGen,lLightGen,lLightGen,1.0f);
	//------------------------------

	//スポットライト減衰計算----------------------(開始)

	//ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積(即ち Cos a)を計算
	float3 LtoV = normalize(spotLPos.xyz - PSInput.worldPos.xyz);
	lLightDirectionCosA = dot(lLightDir, (spotL.direction));

	// 一時的なデバッグ: lLightDirectionCosA だけを返す
	//return float4(lLightDirectionCosA, lLightDirectionCosA, lLightDirectionCosA, 1.0f);

	//スポットライト減衰計算　pow( falloff, ((Cos a - Cos f) / (Cos q - Cos f)))
	lLightGen *= saturate(pow(abs(max(lLightDirectionCosA - spotL.spotParam0, 0.0f) * spotL.spotParam1), spotL.fallOff));

	//最終的なスポットライトの減衰率を返す
	//return float4(lLightGen,lLightGen,lLightGen,1.0f);

	//------------------------------

	//有効距離外だったら減衰率を最大にする処理
	lLightGen *= step(lLightDistancePow2, spotL.rangePow2);

	return float4(lLightGen, lLightGen, lLightGen, 1.0f);

	//距離・スポットライト減衰計算----------------(終了)

	//ディフューズ色計算--------------------------(開始)

	//ディフューズ角度減衰率計算
	DiffuseAngleGen = saturate(dot(Normal, -lLightDir));

	//ライト方向(ワールド座標系)
	float3 WorldLightDir = normalize(spotL.position.xyz - PSInput.worldPos.xyz);

	// ディフューズ角度減衰計算
	DiffuseAngleGen = saturate(dot(Normal, WorldLightDir));

	// 標準的なライティングの寄与
	TotalDiffuse += (float4(spotL.diffuse.xyz, 1.0f) * material.diffuse * DiffuseAngleGen
		+ spotL.ambient) * lLightGen;

	//スペキュラーカラー計算

	//ハーフベクトルの計算
	TempF3 = normalize(V_to_Eye - lLightDir);
	Temp = pow(max(0.0f, dot(Normal, TempF3)), material.power.x);

	//スペキュラーカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラーカラー
	TotalSpecular += Temp * lLightGen.x * (float4(spotL.specular.xyz, 1.0f));

	//スポットライトの処理+++++++++++++++++++++++++++++++++(終了)

	//出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(開始)

	//TotalDiffuse = ライトディフーズカラー蓄積値 + (マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの)
	TotalDiffuse += material.ambientEmissive;

	// SpecularColor = ライトのスペキュラーカラー蓄積値 * マテリアルのスペキュラーカラー
	SpecularDiffuseColor = TotalSpecular * material.specular;

	// 出力カラー計算 = TotalDiffuse * テクスチャカラー + SpecularColor
	float4 retColor;

	TextureDiffuseColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv.xy);
	retColor.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + SpecularDiffuseColor.rgb;

	//アルファ値 = テクスチャアルファ * マテリアルのディフューズアルファ * 不透明度
	retColor.a = TextureDiffuseColor.a * material.diffuse.a * g_base.factorColor.a;

	//出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(終了)

	//出力パラメータを返す
	return retColor;
}