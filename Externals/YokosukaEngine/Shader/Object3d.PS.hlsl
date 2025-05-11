#include "Object3d.hlsli"

// マテリアル
struct Material
{
    float4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

// テクスチャの宣言
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// ピクセルシェーダの出力
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// main関数
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // テクスチャの色を取得する
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    // テクスチャと色を乗算する
    output.color = gMaterial.color * textureColor;
    
    return output;
}