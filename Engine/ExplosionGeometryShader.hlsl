struct INPUT_VERTEX
{
	float4 position : SV_POSITION;
};

struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
};

[maxvertexcount(3)]
void main(triangle INPUT_VERTEX input[3], inout TriangleStream<OUTPUT_VERTEX> outputStream)
{
	// Calculate normal
	// Make a const float distance variable
	// Move the input.position by the distance variable in the normal direction

	//for (uint i = 0; i < 3; i++)
	//{
	//	OUTPUT_VERTEX output;
	//	output.position = input[i].position;
	//	outputStream.Append(output);
	//}
}