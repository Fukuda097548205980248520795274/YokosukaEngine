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
    float4 color[1024];
    
    // 向き
    float4 direction[1024];
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

// スポットライト
struct SpotLight
{
    // 色
    float4 color;
    
    // 位置
    float3 position;
    
    // 輝度
    float intensity;
    
    // 方向
    float3 direction;
    
    // ライトの届く最大距離
    float distance;
    
    // 減衰率
    float decay;
    
    // スポットライトの余弦
    float cosAngle;
    
    // フォールオフの開始値
    float fallofStart;
};
ConstantBuffer<SpotLight> gSpotLight : register(b4);

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
        
        // 入射光
        float directionalLightNdotL0 = dot(normalize(input.normal), -(float3) gDirectionalLight.direction[0]);
        float directionalLightCos0 = pow(directionalLightNdotL0 * 0.5f + 0.5f, 2.0f);
        
        float directionalLightNdotL1 = dot(normalize(input.normal), -(float3) gDirectionalLight.direction[1]);
        float directionalLightCos1 = pow(directionalLightNdotL1 * 0.5f + 0.5f, 2.0f);
        
        float directionalLightNdotL2 = dot(normalize(input.normal), -(float3) gDirectionalLight.direction[2]);
        float directionalLightCos2 = pow(directionalLightNdotL2 * 0.5f + 0.5f, 2.0f);
        
        
        // 入射光の反射ベクトル
        float3 directionalLightHalfVector0 = normalize(-(float3)gDirectionalLight.direction[0] + toEye);
        float directionalLightNDotH0 = dot(normalize(input.normal), directionalLightHalfVector0);
        float directionalLightSpeculerPow0 = pow(saturate(directionalLightNDotH0), gMaterial.shininesse);
        
        float3 directionalLightHalfVector1 = normalize(-(float3) gDirectionalLight.direction[1] + toEye);
        float directionalLightNDotH1 = dot(normalize(input.normal), directionalLightHalfVector1);
        float directionalLightSpeculerPow1 = pow(saturate(directionalLightNDotH1), gMaterial.shininesse);
        
        float3 directionalLightHalfVector2 = normalize(-(float3) gDirectionalLight.direction[2] + toEye);
        float directionalLightNDotH2 = dot(normalize(input.normal), directionalLightHalfVector2);
        float directionalLightSpeculerPow2 = pow(saturate(directionalLightNDotH2), gMaterial.shininesse);
        
         // 拡散反射
        float3 diffuseDirectionalLight =
        gMaterial.color.rgb * textureColor.rgb * 
        (gDirectionalLight.color[0].rgb * directionalLightCos0 +
        gDirectionalLight.color[1].rgb * directionalLightCos1 +
        gDirectionalLight.color[2].rgb * directionalLightCos2);
        
        // 鏡面反射
        float3 speculerDirectionalLight =
        (gDirectionalLight.color[0].rgb * directionalLightSpeculerPow0 +
        gDirectionalLight.color[1].rgb * directionalLightSpeculerPow1 +
        gDirectionalLight.color[2].rgb * directionalLightSpeculerPow2) *
        float3(1.0f, 1.0f, 1.0f);
        
        
        /*-----------
            点光源
        -----------*/
        
        // 点光源への距離
        float distance = length(gPointLight.position - input.worldPosition);
        
        // 逆二乗則による減速係数
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        
        // 入射光
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        float pointLightNdotL = dot(normalize(input.normal), -pointLightDirection);
        float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // 入射光の反射ベクトル
        float3 pointLightHalfVector = normalize(-pointLightDirection + toEye);
        float pointLightNdotH = dot(normalize(input.normal), pointLightHalfVector);
        float pointLightSpeculerPow = pow(saturate(pointLightNdotH), gMaterial.shininesse);
        
        // 拡散反射
        float3 diffusePointLight =
        gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * pointLightCos * gPointLight.intensity * factor;
        
        // 鏡面反射
        float3 speculerPointLight =
        gPointLight.color.rgb * gPointLight.intensity * factor * pointLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        /*------------------
            スポットライト
        ------------------*/
        
        // スポットライトへの距離
        float spotLightDistance = length(gSpotLight.position - input.worldPosition);
        
        // 逆二乗則による減衰係数
        float spotLightFactor = pow(saturate(-spotLightDistance / gSpotLight.distance + 1.0f), gSpotLight.decay);
        
        // 入射光を求める
        float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        float spotLightNdotL = dot(normalize(input.normal), -spotLightDirectionOnSurface);
        float spotLightCos = pow(spotLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // 入射光の反射ベクトル
        float3 spotLightHalfVector = normalize(-spotLightDirectionOnSurface + toEye);
        float spotLightNdotH = dot(normalize(input.normal), spotLightHalfVector);
        float spotLightSpeculerPow = pow(saturate(spotLightNdotH), gMaterial.shininesse);
        
        // フォールオフ
        float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.fallofStart - gSpotLight.cosAngle));
        
        // 拡散反射
        float3 diffuseSpotLight =
        gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * spotLightCos * gSpotLight.intensity * spotLightFactor * falloffFactor;
        
        // 鏡面反射
        float3 speculerSpotLight =
        gPointLight.color.rgb * gSpotLight.intensity * spotLightFactor * falloffFactor * spotLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        // 全て乗算する
        output.color.rgb = 
        diffuseDirectionalLight + speculerDirectionalLight + diffusePointLight + speculerPointLight + diffuseSpotLight + speculerSpotLight;
        
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