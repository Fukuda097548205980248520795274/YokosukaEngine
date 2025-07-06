#include "FullScreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D gTexture : register(t0);
SamplerState gSample : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // テクスチャの色データを渡す
    output.color = gTexture.Sample(gSample, input.texcoord);
    
    // 周囲を0.0f　中心になるほど明るくなるようにする
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    
    // correctだけでは中心の最大値が低いので調整する
    float vignette = correct.x * correct.y * 16.0f;
    
    // とりあえず0.8乗する
    vignette = saturate(pow(vignette, 0.8f));
    
    // 係数として乗算
    output.color.rgb *= vignette;
    
    
    return output;
}