
struct VertexToPixel
{
    float4 svPos		: SV_POSITION;
    float4 diffuse		: COLOR0;
    float4 specular		: COLOR1;
    float2 uv			: TEXCOORD;
};

struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	float3 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
	float3 normal       : TEXCOORD2;	// 法線( ビュー座標 )
	float4 diffuse      : COLOR0;		// ディフューズカラー
	float3 lightDir     : TEXCOORD3;	// ライト方向(ローカル)
	float3 lightAtPos   : TEXCOORD4;	// ライトから見た座標
	float3 worldPos     : TEXCOORD5;	// 頂点座標(ワールド座標)
	float3 plightPos_vs	: TEXCOORD6;	// ポイントライト位置(ビュー座標)
	float3 plightDir_vs	: TEXCOORD7;	// ポイントライトの方向
	float3 sLightPos_vs : TEXCOORD8;	// スポットライト位置(ビュー座標)
	float3 sLightDir_vs : TEXCOORD9;	// スポットライトの方向(ビュー座標)
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	float4 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
};
