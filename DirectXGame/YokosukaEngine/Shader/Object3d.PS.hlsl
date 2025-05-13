#include "Object3d.hlsli"

// マテリアル
struct Material
{
    // 色
    float4 color;
    
    // ライティング有効化
    int enableLighting;
};
ConstantBuffer<Material> gMaterial : register(b0);

// 平行光源
struct DirectionalLight
{
    // 色
    float4 color;
    
    // 向き
    float3 direction;
    
    // 輝度
    float intensity;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

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
    
    // ライティング適用
    if (gMaterial.enableLighting)
    {
        // 角度で光が入る明るさを求める 0 ~ 1
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // 全て乗算する
        output.color = textureColor * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;

    }
    else
    {
        // ライディング無効
        
         // テクスチャと色を乗算する
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}