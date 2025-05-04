cbuffer UBO : register(b0, space1)
{
    row_major float4x4 ubo_mvp : packoffset(c0);
};


static float4 gl_Position;
static float3 outColor;
static float4 inColor;
static float3 inPos;

struct SPIRV_Cross_Input
{
    float3 inPos : TEXCOORD0;
    float4 inColor : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float3 outColor : TEXCOORD0;
    float4 gl_Position : SV_Position;
};

void main_inner()
{
    outColor = inColor.xyz;
    gl_Position = mul(float4(inPos, 1.0f), ubo_mvp);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    inColor = stage_input.inColor;
    inPos = stage_input.inPos;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.outColor = outColor;
    return stage_output;
}
