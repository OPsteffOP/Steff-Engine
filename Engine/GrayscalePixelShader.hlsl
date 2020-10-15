struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float2 textureCoordinate : TEXCOORD0;
};

Texture2D g_Texture;

SamplerState g_Sampler : register(s0);

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 color = g_Texture.Sample(g_Sampler, input.textureCoordinate);
	float meanValue = (color.r + color.g + color.b) / 3.f;

	return float4(meanValue, meanValue, meanValue, 1.f);
}