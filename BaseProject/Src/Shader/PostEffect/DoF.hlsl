#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 使用テクスチャ
Texture2D srcTexture : register(t0);
SamplerState srcSampler : register(s0);
SamplerState depthSampler : register(s1);
Texture2D depthTexture : register(t1);
SamplerState blur01Sampler : register(s2);
Texture2D blur01Texture : register(t2);
SamplerState blur02Sampler : register(s3);
Texture2D blur02Texture : register(t3);

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	// 被写界深度のフォーカス情報
	float g_focus_end; // ぼかさない距離
	float g_blur_size; // ぼかし距離
};

// main関数
float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//return float4(0.0f,1.0f,0.0f,1.0f);

	float4 colorFrom;
	float4 colorTo;
	float blendRate;
	float fade;
	float depth;

	//colorFrom = srcTexture.Sample(srcSampler, PSInput.uv);

	//return colorFrom;

	// 深度値を取得する
	depth = depthTexture.Sample(depthSampler, PSInput.uv);
	return float4(depth, depth, depth, 1.0f);

	// フォーカス情報からぼやけ率を算出
	if (depth < g_focus_end)
	{
		// フォーカス範囲内
		fade = 0.0f;
	}
	else
	{
		// フォーカス範囲内より奥
		fade = depth / (g_focus_end + g_blur_size);
	}
	// ぼやけ率から色を算出
	if (fade < 0.5f)
	{
		// ぼやけ率が 0.5f 以下の場合は、
		// ぼかし無し画像と弱いぼかし画像を合成する
		colorFrom = srcTexture.Sample(srcSampler, PSInput.uv);
		colorTo = blur01Texture.Sample(blur01Sampler, PSInput.uv);
		// 0.0～0.5 を 0.0～1.0にする
		blendRate = fade * 2.0f;
	}
	else
	{
		// ぼやけ率が 0.5f 以上の場合は、
		// 弱いぼかし画像と強いぼかし画像を合成する
		colorFrom = blur01Texture.Sample(blur01Sampler, PSInput.uv);
		colorTo = blur02Texture.Sample(blur02Sampler, PSInput.uv);
		// 0.5 ～ 1.0 を 0.0～1.0にする
		blendRate = (fade - 0.5f) * 2.0f;
	}
	// ぼやかし具合を合成
	return lerp(colorFrom, colorTo, blendRate);
}