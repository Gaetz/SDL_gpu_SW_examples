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
    float3 _15 = inColor * 1.5f;
    outFragColor.x = _15.x;
    outFragColor.y = _15.y;
    outFragColor.z = _15.z;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    inColor = stage_input.inColor;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.outFragColor = outFragColor;
    return stage_output;
}
