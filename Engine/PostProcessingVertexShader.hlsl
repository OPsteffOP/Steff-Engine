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

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;

	output.position = float4(input.position, 1.f);
	output.textureCoordinate = input.textureCoordinate;

	return output;
}