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

// 点光源
struct PointLight
{
    // 色
    float4 color;
    
    // 位置
    float3 position;
    
    // 輝度
    float intensity;
    
    // ライトの届く最大距離
    float radius;
    
    // 減衰率
    float decay;
};
ConstantBuffer<PointLight> gPointLight : register(b3);

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
        // カメラへの方向
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
        
        /*-------------
            平行光源
        -------------*/
        
        // 平行光源の入射光
        float directionalLightNdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // 平行光源の入射光の反射ベクトル
        float3 directionalLightHalfVector = normalize(-gDirectionalLight.direction + toEye);
        float directionalLightNDotH = dot(normalize(input.normal), directionalLightHalfVector);
        float directionalLightSpeculerPow = pow(saturate(directionalLightNDotH), gMaterial.shininesse);
        
         // 拡散反射（平行光源）
        float3 diffuseDirectionalLight =
        gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intensity;
        
        // 鏡面反射（平行光源）
        float3 speculerDirectionalLight =
        gDirectionalLight.color.rgb * gDirectionalLight.intensity * directionalLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        /*-----------
            点光源
        -----------*/
        
        // 点光源への距離
        float distance = length(gPointLight.position - input.worldPosition);
        
        // 逆二乗則による減速係数
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        
        // 点光源の入射光
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        float pointLightNdotL = dot(normalize(input.normal), -pointLightDirection);
        float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // 点光源の入射光の反射ベクトル
        float3 pointLightHalfVector = normalize(-pointLightDirection + toEye);
        float pointLightNdotH = dot(normalize(input.normal), pointLightHalfVector);
        float pointLightSpeculerPow = pow(saturate(pointLightNdotH), gMaterial.shininesse);
        
        // 拡散反射（点光源）
        float3 diffusePointLight =
        gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * pointLightCos * gPointLight.intensity * factor;
        
        // 鏡面反射（点光源）
        float3 speculerPointLight =
        gPointLight.color.rgb * gPointLight.intensity * factor * pointLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        
        // 全て乗算する
        output.color.rgb = diffuseDirectionalLight + speculerDirectionalLight + diffusePointLight + speculerPointLight;
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