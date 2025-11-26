// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_8FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

#define L_W_MAT g_localWorldMatrix.lwMatrix

// 定数バッファ：スロット4番目
cbuffer cbParam : register(b4)
{
}

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret = (VS_OUTPUT)0;

	// スキンメッシュ用のローカル⇒ワールド変換行列を作成+++( 開始 )
	int4 lBoneIdx;
	float4 lL_W_Mat[3];
	float4 lWeight;
	float4 lWorldPosition;
	float4 lLocalPosition;
	float4 lViewPosition;

	// BONE4
	lBoneIdx = VSInput.blendIndices0;
	lWeight = VSInput.blendWeight0;
	lL_W_Mat[0] = L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
	lL_W_Mat[1] = L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
	lL_W_Mat[2] = L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;

	// BONE8
	lBoneIdx = VSInput.blendIndices1;
	lWeight = VSInput.blendWeight1;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;
	lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;
	lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
	lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;
	// スキンメッシュ用のローカル⇒ワールド変換行列を作成+++( 終了 

	// float3 → float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;
	ret.svPos.xyz = lLocalPosition.xyz;

	// ローカル座標をワールド座標に変換
	lWorldPosition.w = 1.0f;
	lWorldPosition.x = dot(lLocalPosition, lL_W_Mat[0]);
	lWorldPosition.y = dot(lLocalPosition, lL_W_Mat[1]);
	lWorldPosition.z = dot(lLocalPosition, lL_W_Mat[2]);
	ret.worldPos.xyz = lWorldPosition.xyz;

	// 法線をローカル空間からワールド空間へ変換
	ret.normal.x = dot(VSInput.norm, lL_W_Mat[0].xyz);
	ret.normal.y = dot(VSInput.norm, lL_W_Mat[1].xyz);
	ret.normal.z = dot(VSInput.norm, lL_W_Mat[2].xyz);

	// ワールド座標をビュー座標に変換
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos.xyz = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);



	return ret;
}