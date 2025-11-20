// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// ピクセルシェーダーの出力
struct PS_MULTI_OUTPUT
{
	float4 Output : SV_TARGET0;
	float4 Depth : SV_TARGET1;
};

PS_MULTI_OUTPUT main(PS_INPUT PSInput)
{
	PS_MULTI_OUTPUT ret;

	float4 color;

	// テクスチャの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	if (color.a < 0.01f)
	{
		discard;
	}

	// 出力
	ret.Output = float4(color.rgb, color.a);
	// ret.Depth = float4(PSInput.vwPos.z, 0.0f, 0.0f, 1.0f);
	float abs_z = abs(PSInput.vwPos.z);
	
	ret.Depth = float4(abs_z / 3000.0f, 0.0f, 0.0f, 1.0f);
	return ret;

}