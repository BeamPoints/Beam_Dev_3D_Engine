struct PSInput
{
    float4 position : SV_POSITION;
    float4 posVS : POSITION1;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float4 texcoord : TEXCOORD0;
    float3x3 tbn : TEXCOORD1;
    float3 light : TEXCOORD4;
};

struct PSOutput
{
    float4 color : SV_TARGET;
};

Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
Texture2D normalTexture : register(t2);
Texture2D reflectionMap : register(t3);
Texture2D reflectionTexture : register(t4);
SamplerState diffuseSamplerState : register(s0);
SamplerState specularSamplerState : register(s1);
SamplerState normalSamplerState : register(s2);
SamplerState reflectionMapState : register(t3);
SamplerState reflectionSamplerState : register(s4);


PSOutput main(PSInput input)
{
    float4 I_diffuse = diffuseTexture.Sample(diffuseSamplerState, float2(input.texcoord.x, abs(1.0 - input.texcoord.y)));
    float4 I_specular = specularTexture.Sample(specularSamplerState, float2(input.texcoord.x, abs(1.0 - input.texcoord.y)));
    float4 I_normal = normalTexture.Sample(normalSamplerState, float2(input.texcoord.x, abs(1.0 - input.texcoord.y)));

    float3 N_packed = I_normal.rgb;
    float3 N_unpacked = (2.0 * N_packed) - 1.0;

    float3x3 inverseTBN = transpose(input.tbn);

    float3 N_view = mul(inverseTBN, N_unpacked);

    float3 L = input.light;
    float3 N = N_view; // input.normal;
    float3 V = input.posVS.xyz;
    
    float3 Lnorm = normalize(L);
    float3 Nnorm = normalize(N);
    float3 Vnorm = normalize(V);

    float3 R = -reflect(L, Nnorm);
    float3 Rnorm = normalize(R);
    
    float3 rv = reflect(V, input.normal);
    float k_reflect = k_reflect.r
    float4 i_reflect = reflectionTexture.Sample(reflectionSamplerState, rv);

    float4 ambient_I = float4(226.0 / 255.0, 0.0 / 255.0, 116.0 / 255.0, 255.0 / 255.0);
    float ambient_k = 0.0;
                        
    float4 lambert_I = I_diffuse;
    float lambert_k = 1.0;
    float lambert_cos = saturate(dot(-Lnorm, Nnorm));
        
    float4 specular = float4(0.0, 0.0, 0.0, 1.0);

    if (lambert_cos >= 0)
    {
        float specular_n_min = 1.0;
        float specular_n_max = 255;
        float specular_n = (I_specular.r * specular_n_max) + specular_n_min;
        float4 specular_I = float4(0.0, 0.0, 1.0, 1.0);
        float specular_k = I_specular.r;
        float specular_cos = pow(saturate(dot(Rnorm, Vnorm)), specular_n);
        
        specular = specular_I * specular_k * specular_cos;
    }

    float4 ambient = ambient_I * ambient_k;
    float4 lambert = lambert_I * lambert_k * lambert_cos;

    PSOutput output;
    // output.color = float4((input.position.xy / float2(1920, 1080)), 0.0, 1.0);
    output.color = ambient + lambert + specular;
    // output.color = clamp(output.color, 0.0, 1.0);
    output.color = saturate(output.color);

    output.color = i_reflect;
    
    return output;
}