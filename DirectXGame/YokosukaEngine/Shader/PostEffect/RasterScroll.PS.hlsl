#include "Fullscreen.hlsli"

// ラスタースクロール
struct RasterScroll
{
    // 振幅
    float amplitude;
    
    // 周波数
    float frequency;
    
    // 位相オフセット
    float phaseOffset;
    
    // スクロール速度
    float scrollSpeed;
    
    // 時間
    float time;
};
ConstantBuffer<RasterScroll> gRasterScroll : register(b0);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // スクリーン座標のY成分を取得 (0-1の範囲に正規化されている場合も考慮)
    // 通常はSV_Position.y を用いるが、ここでは input.uv.y をY座標の代わりとして使う例
    // 環境によってSV_Position.y を使うか、頂点シェーダーから補間された何らかのY値を使うか変わる
    float screenY = input.texcoord.y;

    // Y座標に基づいたオフセット量を計算
    // Y座標の範囲が0-1の場合、frequencyの調整が必要
    float offsetX = gRasterScroll.amplitude *
    sin(screenY * gRasterScroll.frequency * 2 * 3.14159 + gRasterScroll.phaseOffset + gRasterScroll.time * gRasterScroll.scrollSpeed);

    // UV座標をオフセット
    float2 finalUV = input.texcoord;
    finalUV.x += offsetX; // X方向にオフセットを適用

    // オフセットされたUV座標でテクスチャをサンプリング
    output.color = gTexture.Sample(gSampler, finalUV);
    
    return output;
}