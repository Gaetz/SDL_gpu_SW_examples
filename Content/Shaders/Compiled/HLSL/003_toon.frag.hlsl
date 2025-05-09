Texture2D<float4> samplerColorMap : register(t1, space2);
SamplerState _samplerColorMap_sampler : register(s1, space2);

static float3 inColor;
static float3 inNormal;
static float3 inLightVec;
static float3 inViewVec;
static float4 outFragColor;

struct SPIRV_Cross_Input
{
    float3 inNormal : TEXCOORD0;
    float3 inColor : TEXCOORD1;
    float3 inViewVec : TEXCOORD2;
    float3 inLightVec : TEXCOORD3;
};

struct SPIRV_Cross_Output
{
    float4 outFragColor : SV_Target0;
};

void main_inner()
{
    float3 color = lerp(inColor, dot(float3(0.2125999927520751953125f, 0.715200006961822509765625f, 0.072200000286102294921875f), inColor).xxx, 0.64999997615814208984375f.xxx);
    float3 ambient = color * 1.0f.xxx;
    float3 N = normalize(inNormal);
    float3 L = normalize(inLightVec);
    float3 V = normalize(inViewVec);
    float3 R = reflect(-L, N);
    float3 diffuse = color * max(dot(N, L), 0.0f);
    float3 specular = 0.75f.xxx * pow(max(dot(R, V), 0.0f), 16.0f);
    outFragColor = float4((ambient + (diffuse * 1.75f)) + specular, 1.0f);
    float intensity = dot(N, L);
    float shade = 1.0f;
    shade = (intensity < 0.5f) ? 0.75f : shade;
    shade = (intensity < 0.3499999940395355224609375f) ? 0.60000002384185791015625f : shade;
    shade = (intensity < 0.25f) ? 0.5f : shade;
    shade = (intensity < 0.100000001490116119384765625f) ? 0.25f : shade;
    float3 _109 = (inColor * 3.0f) * shade;
    outFragColor.x = _109.x;
    outFragColor.y = _109.y;
    outFragColor.z = _109.z;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    inColor = stage_input.inColor;
    inNormal = stage_input.inNormal;
    inLightVec = stage_input.inLightVec;
    inViewVec = stage_input.inViewVec;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.outFragColor = outFragColor;
    return stage_output;
}
