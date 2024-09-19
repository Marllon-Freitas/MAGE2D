// transformation and projection matrix
cbuffer ConstantBuffer
{
    float4x4 WorldViewProj;
}


// input vertex structure
struct VertexIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

// output vertex structure
struct VertexOut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};


// main vertex shader program
VertexOut main(VertexIn vIn)
{
    VertexOut vOut;

    // transform vertices to screen coordinates
    vOut.Pos = mul(float4(vIn.Pos, 1.0f), WorldViewProj);
    
    // keep colors unchanged
    vOut.Color = vIn.Color;

    // keep texture coordinates unchanged
    vOut.Tex = vIn.Tex;

    return vOut;
}
