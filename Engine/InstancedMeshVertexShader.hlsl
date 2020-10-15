struct INPUT_VERTEX
{
	float3 position : POSITION;
	float2 textureCoordinate : TEXCOORD0;
	matrix worldMatrix : WORLD;
};

struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float2 textureCoordinate : TEXCOORD0;
};

cbuffer MeshConstantBuffer : register(b0)
{
	matrix g_ParentWorldMatrix;
}

cbuffer CameraConstantBuffer : register(b1)
{
	matrix g_ViewProjectionMatrix;
}

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output;

	output.position = mul(mul(mul(float4(input.position, 1.f), input.worldMatrix), g_ParentWorldMatrix), g_ViewProjectionMatrix);
	output.textureCoordinate = input.textureCoordinate;

	return output;
}