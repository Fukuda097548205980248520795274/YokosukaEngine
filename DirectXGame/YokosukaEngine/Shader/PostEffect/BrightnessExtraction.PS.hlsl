#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Luminance
{
    // 輝度の閾値
    float threshold;
};
ConstantBuffer<Luminance> gLuminance : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // 1. テクスチャから現在のピクセルの色をサンプリング
    output.color = gTexture.Sample(gSampler, input.texcoord);

    // 2. RGBカラーから輝度（明るさ）を計算する
    // 一般的な方法として、NTSC係数（Rec. 709の輝度計算）を使用します。
    // R * 0.299 + G * 0.587 + B * 0.114
    // アルファチャンネルは輝度計算には含めません。
    float luminance = dot(output.color.rgb, float3(0.299, 0.587, 0.114));
    // または、もっと単純に最大コンポーネントを使う方法もありますが、NTSC係数の方が視覚的に自然なことが多いです。
    // float luminance = max(max(color.r, color.g), color.b);

    // 3. 輝度が閾値を超えているか判定する
    if (luminance <= gLuminance.threshold)
    {
        // 閾値以下の場合は、完全に黒を出力 (アルファは通常1.0)
        output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    
    return output;
}