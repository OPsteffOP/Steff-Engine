cbuffer VSBuffer : register(b0)
{
	matrix g_WorldViewProjectionMatrix;
	float g_Time;
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 0.f, 0.f, 1.0f);
}