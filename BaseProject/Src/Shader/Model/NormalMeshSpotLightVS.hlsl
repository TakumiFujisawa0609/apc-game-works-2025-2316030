// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// 定数バッファ：スロット4番目
cbuffer cbParam : register(b4)
{
	float4 g_camera_pos;
	float g_fog_start;
	float g_fog_end;
	float2 padding;
}

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;

	// float3 → float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;
	ret.svPos.xyz = lLocalPosition.xyz;

	// ローカル座標をワールド座標に変換(剛体)
	lWorldPosition.w = 1.0f;
	lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
	ret.worldPos.xyz = lWorldPosition.xyz;

	// ワールド座標をビュー座標に変換
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos.xyz = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
	// 出力パラメータを返す
	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
	// 
	// UV座標
	ret.uv.x = VSInput.uv0.x;
	ret.uv.y = VSInput.uv0.y;
	

	// 法線をローカル空間からワールド空間へ変換
	ret.normal = normalize(
		mul(VSInput.norm, (float3x3)g_base.localWorldMatrix));
	
	// ディフューズカラー
	ret.diffuse = VSInput.diffuse;

	// ライト方向(ローカル)
	ret.lightDir = float3(1.0f, 1.0f, 1.0f);

	// ライトから見た座標
	ret.lightAtPos = float3(1.0f, 1.0f, 1.0f);


	// フォグ
	VsFog fog;

	// フォグの距離
	float fogRange = g_fog_end - g_fog_start;

	// フォグend
	if (fogRange > 0.0001f)
	{
		fog.linearAdd = g_fog_end / fogRange;
		fog.linearDiv = -1.0f / fogRange;
	}
	else{
		fog.linearAdd = 1.0f;
		fog.linearDiv = 0.0f;
	}
	

	// フォグの強さ
	// カメラ位置と頂点位置の距離を求める (ワールド座標)
	float distance = length(lWorldPosition.xyz - g_camera_pos.xyz);

	// フォグの距離範囲 (ゼロ除算対策も兼ねる)
	float fog_density = g_fog_end - g_fog_start;

	// Start == End など、フォグが定義されていない場合はフォグを適用しない (1.0)
	if (fog_density <= 0.0001f)
	{
		ret.fogFactor = 1.0f;
	}
	else
	{
		// ユーザーの計算式: (終了距離 - 距離) / (終了距離 - 開始距離) を適用し、[0.0, 1.0] にクランプ
		// distance <= start の時 1.0、distance >= end の時 0.0 となる
		ret.fogFactor = clamp((g_fog_end - distance) / fog_density, 0.0f, 1.0f);
	}

	return ret;
}