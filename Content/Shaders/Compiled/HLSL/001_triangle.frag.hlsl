static float4 outFragColor;
static float3 inColor;

struct SPIRV_Cross_Input
{
    float3 inColor : TEXCOORD0;
};

struct SPIRV_Cross_Output
{
    float4 outFragColor : SV_Target0;
};

void main_inner()
{
    outFragColor = float4(inColor, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    inColor = stage_input.inColor;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.outFragColor = outFragColor;
    return stage_output;
}
