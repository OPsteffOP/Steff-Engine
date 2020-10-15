struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float3 worldPosition : WORLD_POSITION;
	float2 texCoord : TEXCOORD0;
};

SamplerState g_Sampler : register(s0);
Texture2D variable4;

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
float4 variable6 = variable4.SampleLevel(g_Sampler, input.texCoord, 0);
return variable6;

}
