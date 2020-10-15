struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float2 textureCoordinate : TEXCOORD0;
};

cbuffer DataConstantBuffer : register(b0)
{
	float2 g_SplashUVSize; // value depending on how big the splash size is in comparison the the plane size (example: 0.5f if the splash size is half the plane size)
	float2 g_SplashUVLeftTop;
};

Texture2D g_DiffuseTexture : register(t0);
Texture2D g_SplashTexture : register(t1);

SamplerState g_Sampler : register(s0);

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 pixelColor = g_DiffuseTexture.Sample(g_Sampler, input.textureCoordinate);
	if ((input.textureCoordinate.x >= g_SplashUVLeftTop.x - (g_SplashUVSize.x / 2.f) && input.textureCoordinate.x <= g_SplashUVLeftTop.x + (g_SplashUVSize.x / 2.f)) &&
		(input.textureCoordinate.y >= g_SplashUVLeftTop.y - (g_SplashUVSize.y / 2.f) && input.textureCoordinate.y <= g_SplashUVLeftTop.y + (g_SplashUVSize.y / 2.f)))
	{
		float2 splashTextureCoordinate = (input.textureCoordinate - g_SplashUVLeftTop) / g_SplashUVSize;
		splashTextureCoordinate = 0.5f - splashTextureCoordinate;
		pixelColor += g_SplashTexture.Sample(g_Sampler, splashTextureCoordinate);
	}

	return saturate(pixelColor);
}