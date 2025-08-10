#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct ScanlineParams
{
    // 画面サイズ
    float2 screenSize;
    
    // 走査線の太さ
    float thickness;
    
    // 走査線の濃さ (0.0f ~ 1.0f)
    float intensity;
    
    // 走査線の間隔
    float spacing;
    
    // 時間
    float time;
    
    // 移動速度
    float speed;
};
ConstantBuffer<ScanlineParams> gScanlineParams : register(b0);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    // 出力
    PixelShaderOutput output;
    
    float4 originalColor = gTexture.Sample(gSampler, input.texcoord);

    // ピクセル単位のY座標を計算
    float pixelY = input.texcoord.y * gScanlineParams.screenSize.y;

    // 時間と速度に基づいて、走査線のオフセットを計算
    // _Timeはゲームの経過時間（秒単位など）
    // _Speedは走査線の移動速度
    float yOffset = fmod(gScanlineParams.time * gScanlineParams.speed, gScanlineParams.spacing);

    // Y座標からオフセットを引いて、走査線の位置をずらす
    float yWithOffset = pixelY - yOffset;

    // 走査線の判定にオフセット付きのY座標を使用
    if (fmod(yWithOffset, gScanlineParams.spacing) < gScanlineParams.thickness)
    {
        output.color = originalColor * (1.0 - gScanlineParams.intensity);
    }
    else
    {
        output.color = originalColor;
    }
    
    return output;
}