cbuffer UBO : register(b0, space1)
{
    row_major float4x4 ubo_mvp : packoffset(c0);
    float4 ubo_lightPos : packoffset(c4);
};


static float4 gl_Position;
static float3 outNormal;
static float3 inNormal;
static float3 outColor;
static float3 inColor;
static float3 inPos;
static float3 outLightVec;
static float3 outViewVec;

struct SPIRV_Cross_Input
{
    float3 inPos : TEXCOORD0;
    float3 inNormal : TEXCOORD1;
    float3 inColor : TEXCOORD2;
};

struct SPIRV_Cross_Output
{
    float3 outNormal : TEXCOORD0;
    float3 outColor : TEXCOORD1;
    float3 outViewVec : TEXCOORD2;
    float3 outLightVec : TEXCOORD3;
    float4 gl_Position : SV_Position;
};

void main_inner()
{
    outNormal = inNormal;
    outColor = inColor;
    gl_Position = mul(float4(inPos, 1.0f), ubo_mvp);
    float4 pos = mul(float4(inPos, 1.0f), ubo_mvp);
    outNormal = mul(inNormal, float3x3(ubo_mvp[0].xyz, ubo_mvp[1].xyz, ubo_mvp[2].xyz));
    float3 lPos = mul(ubo_lightPos.xyz, float3x3(ubo_mvp[0].xyz, ubo_mvp[1].xyz, ubo_mvp[2].xyz));
    outLightVec = lPos - pos.xyz;
    outViewVec = -pos.xyz;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    inNormal = stage_input.inNormal;
    inColor = stage_input.inColor;
    inPos = stage_input.inPos;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.outNormal = outNormal;
    stage_output.outColor = outColor;
    stage_output.outLightVec = outLightVec;
    stage_output.outViewVec = outViewVec;
    return stage_output;
}
