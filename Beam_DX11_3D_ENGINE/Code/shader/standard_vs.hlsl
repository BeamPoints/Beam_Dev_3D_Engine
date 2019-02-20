cbuffer Data
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

struct VSInput
{
    uint   vertexId : SV_VertexID;
    float4 position : POSITION0;
    float3 normal   : NORMAL0;
    float3 tangent  : TANGENT0;
    float4 texcoord : TEXCOORD0;
};

struct VSOutput
{
    float4   position : SV_POSITION;
    float4   posVS    : POSITION1;
    float3   normal   : NORMAL0;
    float3   tangent  : TANGENT0;
    float4   texcoord : TEXCOORD0;
    float3x3 tbn      : TEXCOORD1;
    float3   light    : TEXCOORD4;
};

VSOutput main(VSInput input)
{
    float3 light = float3(0, -1, 1);

    VSOutput output;
    output.posVS    = mul(view, mul(world, input.position));
    output.position = mul(projection, output.posVS);
    output.normal   = mul((float3x3) view, mul((float3x3) world, input.normal));
    output.tangent  = mul((float3x3) view, mul((float3x3) world, input.tangent));
    output.texcoord = input.texcoord;
    output.light    = mul((float3x3) view, light);

    float3 normal    = normalize(output.normal);
    float3 tangent   = normalize(output.tangent);
    float3 bitangent = normalize(cross(tangent, normal));
    
    output.tbn = float3x3(tangent, bitangent, normal);

	return output;
}