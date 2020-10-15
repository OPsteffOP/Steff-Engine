struct INPUT_VERTEX
{
	float3 position : POSITION;
	float2 textureCoordinate : TEXCOORD0;
};

struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float2 textureCoordinate : TEXCOORD0;
};

cbuffer MeshConstantBuffer : register(b0)
{
	matrix g_WorldMatrix;
}

cbuffer CameraConstantBuffer : register(b1)
{
	matrix g_ViewProjectionMatrix;
}

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output;

	output.position = mul(mul(float4(input.position, 1.f), g_WorldMatrix), g_ViewProjectionMatrix);
	output.textureCoordinate = input.textureCoordinate;

	return output;
}