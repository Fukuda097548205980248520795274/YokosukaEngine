#include "Object3d.hlsli"

// マテリアル
struct Material
{
    // 色
    float4 color;
    
    // ライティング有効化
    int enableLighting;
    
    // UVトランスフォーム
    float4x4 uvTransform;
    
    // 光沢度
    float shininesse;
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

// カメラ
struct Camera
{
    float3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b2);

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
    
    // 座標変換したテクスチャ　（テクスチャ座標系を4次元に変換する）
    float4 transfomedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    
    // 座標変換したテクスチャ座標系で、テクスチャの色を取得する
    float4 textureColor = gTexture.Sample(gSampler, transfomedUV.xy);
    
    // テクスチャの色のα値が0.0fのときにPixelを棄却する
    if (textureColor.a == 0.0f)
    {
        discard;
    }
    
    // ライティング適用
    if (gMaterial.enableLighting)
    {
        // 角度で光が入る明るさを求める 0 ~ 1
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // カメラへの方向
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
        // 入射光の反射ベクトル
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float speculerPow = pow(saturate(NDotH), gMaterial.shininesse);
        
        // 拡散反射
        float3 diffuse = 
        gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        // 鏡面反射
        float3 speculer = 
        gDirectionalLight.color.rgb * gDirectionalLight.intensity * speculerPow * float3(1.0f, 1.0f, 1.0f);
        
        // 全て乗算する
        output.color.rgb = diffuse + speculer;
        output.color.a = gMaterial.color.a * textureColor.a;

    }
    else
    {
        // ライディング無効
        
        // テクスチャと色を乗算する
        output.color = gMaterial.color * textureColor;
    }
    
    // 出力する色のα値が0.0fのときにPixelを棄却する
    if (output.color.a == 0.0f)
    {
        discard;
    }
    
    return output;
}