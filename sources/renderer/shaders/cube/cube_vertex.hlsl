cbuffer MVP : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

struct VSInput
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

PSInput main(VSInput input)
{
    PSInput output;
    float4 worldPos = float4(input.position, 1.0);
    output.position = mul(worldPos, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.color = input.color;
    return output;
}
