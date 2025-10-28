// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_spot_light_pos;
	float4 g_spot_light_dir;
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

	// スポットライトの位置をc＋＋から取得(ワールド空間のものなのでビュー座標系のものに変換する)
	//ret.sLightPos_vs = g_spot_light_pos.xyz;
	ret.sLightPos_vs = mul(float4(g_spot_light_pos.xyz, 1.0f), g_base.viewMatrix).xyz;

	// スポットライトの向き
	//ret.sLightDir_vs = g_spot_light_dir.xyz;
	ret.sLightDir_vs = mul(float4(g_spot_light_dir.xyz, 0.0f), g_base.viewMatrix).xyz;
	// 方向ベクトルは正規化が必要
	ret.sLightDir_vs = normalize(ret.sLightDir_vs);

	return ret;
}