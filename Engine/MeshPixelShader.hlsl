struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float2 textureCoordinate : TEXCOORD0;
};

cbuffer Highlight : register(b0)
{
	float4 g_HighlightColor;
};

Texture2D g_DiffuseTexture : register(t0);
SamplerState g_Sampler : register(s0);

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	return saturate(g_DiffuseTexture.Sample(g_Sampler, input.textureCoordinate) + g_HighlightColor);
}