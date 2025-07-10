#include "Fullscreen.hlsli"

struct Grow
{
    float bloomThreshold;
    float bloomIntensity;
};
ConstantBuffer<Grow> gGrow : register(b0);

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // 入力テクスチャから現在のピクセルの色を取得
    float4 texColor = gTexture.Sample(gSampler, input.texcoord);

    // 輝度を計算 (Rec.709の輝度計算式)
    // RGBそれぞれの寄与度を変えることで、人間の目の明るさの知覚に近い輝度が得られる
    float luminance = dot(texColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));

    // 輝度が閾値を超えているかチェック
    if (luminance > gGrow.bloomThreshold)
    {
        // 閾値を超えている場合、元の色を出力
        // 輝度に基づいて色を乗算することで、より明るい部分を強調することも可能
        // return color * (luminance - g_bloomThreshold); // オプション：閾値からの差分で強度を調整
        output.color = texColor;
    }
    else
    {
        // 閾値以下の場合、黒を出力 (グローの元にならない)
        output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    return output;
}