struct OutputVertex
{
    float4 position : SV_Position;
    float3 worldpos : WORLDPOS;
};

float4 main(OutputVertex input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}