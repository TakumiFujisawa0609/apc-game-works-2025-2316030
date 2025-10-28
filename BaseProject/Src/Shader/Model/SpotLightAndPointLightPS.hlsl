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
	float4 g_ambient_color1;
	float4 g_color;
	float3 g_light_dir;
	float dummy;
	float4 g_ambient_color;
	float3 g_light_pos;
	float g_light_attenuation;
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
	//float3 spotLPos = spotL.position;

	//float3 spotLPos = PSInput.sLightPos_vs;
	float3 spotLPos = spotL.position;
	float3 spotLDir = PSInput.sLightDir_vs;
	//float3 spotLDir = spotL.direction;

	float4 TextureDiffuseColor;
	float4 SpecularDiffuseColor;
	float3 Normal;
	float DiffuseAngleGen;
	float4 TotalDiffuse;
	float4 TotalSpecular;
	float3 V_to_Eye;
	float3 TempF3;
	float Temp;
	float3 lLightTemp;
	float lLightDistancePow2;
	float lLightGen;
	float3 lLightDir;
	float lLightDirectionCosA;

	//法線の準備
	Normal = normalize(PSInput.normal);

	//頂点座標から視点へのベクトルを正規化
	V_to_Eye = normalize(-PSInput.vwPos);
	//V_to_Eye = normalize(-PSInput.worldPos);

	//ディフューズカラーとスペキュラーカラーの蓄積値を初期化
	TotalDiffuse = g_diff_color;	 //(初期色)
	TotalSpecular = g_ambient_color; //(初期色)

	////スポットライトの処理+++++++++++++++++++++++++++++++++(開始)

	////ライト方向ベクトルの計算
	//lLightTemp = PSInput.vwPos.xyz - spotLPos.xyz;
	//lLightDir = normalize(lLightTemp);

	//// スポットライトからピクセルへ向かうビュー空間ベクトル
	///*float3 LtoP_vec_VS = PSInput.vwPos.xyz - spotLPos.xyz;

	//return float4(abs(spotLDir.x), abs(spotLDir.y), abs(spotLDir.z), 1.0f);*/
	//return float4(abs(lLightDir.x), abs(lLightDir.y), abs(lLightDir.z), 1.0f);
	////return float4(lLightDir.xyz, 1.0f);

	////距離・スポットライト減衰値計算-------------(開始)

	////距離減衰計算------------------

	////頂点とライト位置との距離の二乗を求める
	//float3 lsLightTemp = spotLPos.xyz - PSInput.worldPos.xyz;
	//lLightDistancePow2 = dot(lsLightTemp, lsLightTemp);

	////減衰率の計算　lLightGen = 1 / （減衰値0 + 減衰値1 * ( 距離 * 距離) ）
	//lLightGen = 1.0f / (spotL.attenuation0 + spotL.attenuation1 * sqrt(lLightDistancePow2) + spotL.attenuation2 * lLightDistancePow2);

	////return float4(lLightGen,lLightGen,lLightGen,1.0f);
	////------------------------------

	////スポットライト減衰計算----------------------(開始)

	//////ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積(即ち Cos a)を計算
	////float3 LtoV = normalize(PSInput.worldPos.xyz - spotLPos.xyz);
	////lLightDirectionCosA = dot(lLightDir, g_spot_light_dir);

	//////return float4(g_spot_light_dir.x, g_spot_light_dir.y, g_spot_light_dir.z, 1.0f);
	//////return float4(spotL.direction.x, spotL.direction.y, spotL.direction.z, 1.0f);

	////// 一時的なデバッグ: lLightDirectionCosA だけを返す
	////return float4(lLightDirectionCosA, lLightDirectionCosA, lLightDirectionCosA, 1.0f);

	//////スポットライト減衰計算　pow( falloff, ((Cos a - Cos f) / (Cos q - Cos f)))
	////lLightGen *= saturate(pow(abs(max(lLightDirectionCosA - spotL.spotParam0, 0.0f) * spotL.spotParam1), spotL.fallOff));

	////return float4(lLightGen,lLightGen,lLightGen,1.0f);

	////------------------------------

	////有効距離外だったら減衰率を最大にする処理
	//lLightGen *= step(lLightDistancePow2, spotL.rangePow2);

	////距離・スポットライト減衰計算----------------(終了)

	////ディフューズ色計算--------------------------(開始)

	////ディフューズ角度減衰率計算
	//DiffuseAngleGen = saturate(dot(Normal, -lLightDir));

	//// 標準的なライティングの寄与
	//TotalDiffuse += (float4(spotL.diffuse.xyz, 1.0f) * material.diffuse * DiffuseAngleGen
	//	+ spotL.ambient) * lLightGen;

	////スペキュラーカラー計算

	////ハーフベクトルの計算
	//TempF3 = normalize(V_to_Eye - lLightDir);
	//Temp = pow(max(0.0f, dot(Normal, TempF3)), material.power.x);

	////スペキュラーカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラーカラー
	//TotalSpecular += Temp * lLightGen.x * (float4(spotL.specular.xyz, 1.0f));

	////スポットライトの処理+++++++++++++++++++++++++++++++++(終了)

	////出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(開始)

	////TotalDiffuse = ライトディフーズカラー蓄積値 + (マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの)
	//TotalDiffuse += material.ambientEmissive;

	//// SpecularColor = ライトのスペキュラーカラー蓄積値 * マテリアルのスペキュラーカラー
	//SpecularDiffuseColor = TotalSpecular * material.specular;

	//// 出力カラー計算 = TotalDiffuse * テクスチャカラー + SpecularColor
	//float4 retColor;

	//TextureDiffuseColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv.xy);
	//retColor.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + SpecularDiffuseColor.rgb;

	////アルファ値 = テクスチャアルファ * マテリアルのディフューズアルファ * 不透明度
	//retColor.a = TextureDiffuseColor.a * material.diffuse.a * g_base.factorColor.a;

	////出力カラー計算++++++++++++++++++++++++++++++++++++++++++++++++(終了)

	////出力パラメータを返す
	//return retColor;


	//スポットライトの処理+++++++++++++++++++++++++++++++++(開始)

	// [1] ライト方向ベクトルの計算 (ピクセルからライトへ: Pixel -> Light)
	// lightPos/PSInput.vwPosがビュー空間で統一されていることを前提
	//

	 // 1. ピクセルからライトへのベクトルを計算
	float3 lightVec = g_light_pos - PSInput.worldPos;

	// 2. ライトへの距離を計算
	float distance = length(lightVec);

	// 3. ライトの方向ベクトルを正規化
	float3 pointLightDir = normalize(lightVec);

	// 4. 法線とライト方向の内積を計算 (拡散光)
	// saturateは結果を0.0～1.0の範囲にクランプする関数
	float lightDot = saturate(dot(PSInput.normal, pointLightDir));

	// 5. 距離による光の減衰を計算
	// (1.0 / (1.0 + 減衰係数 * 距離^2)) という一般的な式
	float attenuation = 1.0f / (1.0f + g_light_attenuation * distance * distance);

	// 6. 最終的な色を計算
	// (テクスチャ色 * 拡散光 * 減衰) + 環境光
	float3 rgb = (color.rgb * g_color.rgb * lightDot * attenuation) + g_ambient_color.rgb;


return float4(rgb, color.a);

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}