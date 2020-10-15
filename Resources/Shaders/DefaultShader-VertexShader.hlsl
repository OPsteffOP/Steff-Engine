struct INPUT_VERTEX
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float3 worldPosition : WORLD_POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer MeshConstantBuffer : register(b0)
{
	matrix g_WorldMatrix;
};

cbuffer CameraConstantBuffer : register(b1)
{
	matrix g_ViewProjectionMatrix;
};

SamplerState g_Sampler : register(s0);


OUTPUT_VERTEX main(INPUT_VERTEX input)
{
float3 variable4 = float3(0, 0, 0);
input.position += variable4;

	OUTPUT_VERTEX output;
	output.position = mul(mul(float4(input.position, 1.f), g_WorldMatrix), g_ViewProjectionMatrix);
	output.worldPosition = input.position;
	output.texCoord = input.texCoord;

	return output;
}
