struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float3 worldPosition : WORLD_POSITION;
	float2 texCoord : TEXCOORD0;
};

SamplerState g_Sampler : register(s0);

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
float4 variable0 = float4(0.5, 0, 0.5, 1);
return variable0;

}
