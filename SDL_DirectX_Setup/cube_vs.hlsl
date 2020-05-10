#pragma pack_matrix(row_major)

struct InputVertex
{
    float3 position : POSITION;
};

struct OutputVertex
{
    float4 position : SV_Position;
    float3 worldpos : WORLDPOS;
};

cbuffer WORLD : register(b0)
{
    float4x4 worldmat;
    float4x4 viewmat;
    float4x4 projectionMat;
};

OutputVertex main(InputVertex input)
{
    OutputVertex output;

    output.position = float4(input.position, 1);
    output.position = mul(output.position, worldmat);
    output.worldpos = output.position.xyz;
    output.position = mul(output.position, viewmat);
    output.position = mul(output.position, projectionMat);
    
    return output;
}