
struct VS_INPUT
{
    float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer TransformData : register(b0)
{
    // ��켱
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProjection;
    
}

// IA - VS - RS - PS - OM
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // WVP
    float4 position = mul(input.position, matWorld); // World Space
    position = mul(position, matView); // View Space
    position = mul(position, matProjection); // Projection Space
    
    output.position = position;
    output.color = input.color;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{	
    return input.color;
}