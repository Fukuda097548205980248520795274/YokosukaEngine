#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main()
{
    PixelShaderOutput output;
    
    // âBÇ∑ÇΩÇﬂêFÇ»Çµ
    output.color = float4(0.0f,0.0f,0.0f,1.0f);
    
    return output;
}