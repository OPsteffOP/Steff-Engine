cbuffer MeshConstantBuffer : register(b0)
{
	matrix g_WorldMatrix;
}

cbuffer CameraConstantBuffer : register(b1)
{
	matrix g_ViewProjectionMatrix;
}

float4 main(float3 pos : POSITION) : SV_POSITION
{
	return mul(float4(pos, 1.f), g_ViewProjectionMatrix);
}