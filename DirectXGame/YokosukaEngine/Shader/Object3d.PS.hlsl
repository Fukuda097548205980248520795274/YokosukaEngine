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
    
    // 設置したライトの数
    uint setNum;
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
    float4 color[1024];
    
    // 位置
    float4 position[1024];
    
    // ライトの届く最大距離
    float radius[1024];
    
    // 減衰率
    float decay[1024];
    
    // 設置したライトの数
    uint setNum;
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
        
        // 平行光源の拡散反射の色
        float3 diffuseDirectionalLightColor = { 0.0f, 0.0f, 0.0f };
        
        // 平行光源の鏡面反射の色
        float3 speculerDirectionalLightColor = { 0.0f, 0.0f, 0.0f };
        
        for (uint i = 0; i < gDirectionalLight.setNum; i++)
        {
            // 入射光
            float directionalLightNdotL = dot(normalize(input.normal), -(float3) gDirectionalLight.direction[i]);
            float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
            
             // 入射光の反射ベクトル
            float3 directionalLightHalfVector = normalize(-(float3) gDirectionalLight.direction[i] + toEye);
            float directionalLightNDotH = dot(normalize(input.normal), directionalLightHalfVector);
            float directionalLightSpeculerPow = pow(saturate(directionalLightNDotH), gMaterial.shininesse);
            
            diffuseDirectionalLightColor += gDirectionalLight.color[i].rgb * directionalLightCos;
            speculerDirectionalLightColor += gDirectionalLight.color[i].rgb * directionalLightSpeculerPow;
        }
        
         // 拡散反射
        float3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * diffuseDirectionalLightColor;
        
        // 鏡面反射
        float3 speculerDirectionalLight = speculerDirectionalLightColor * float3(1.0f, 1.0f, 1.0f);
        
        
        /*-----------
            点光源
        -----------*/
        
        // 平行光源の拡散反射の色
        float3 diffusePointLightColor = { 0.0f, 0.0f, 0.0f };
        
        // 平行光源の鏡面反射の色
        float3 speculerPointLightColor = { 0.0f, 0.0f, 0.0f };
        
        for (uint j = 0; j < gPointLight.setNum; j++)
        {
            // 点光源への距離
            float distance = length((float3) gPointLight.position[j] - input.worldPosition);
        
            // 逆二乗則による減速係数
            float factor = pow(saturate(-distance / gPointLight.radius[j] + 1.0f), gPointLight.decay[j]);
        
            // 入射光
            float3 pointLightDirection = normalize(input.worldPosition - (float3) gPointLight.position[j]);
            float pointLightNdotL = dot(normalize(input.normal), -pointLightDirection);
            float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
        
            // 入射光の反射ベクトル
            float3 pointLightHalfVector = normalize(-pointLightDirection + toEye);
            float pointLightNdotH = dot(normalize(input.normal), pointLightHalfVector);
            float pointLightSpeculerPow = pow(saturate(pointLightNdotH), gMaterial.shininesse);
            
            diffusePointLightColor += gPointLight.color[j].rgb * pointLightCos * factor;
            speculerPointLightColor += gPointLight.color[j].rgb * factor * pointLightSpeculerPow;

        }
        
        // 拡散反射
        float3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * diffusePointLightColor;
        
        // 鏡面反射
        float3 speculerPointLight = speculerPointLightColor * float3(1.0f, 1.0f, 1.0f);
        
        
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
        gSpotLight.color.rgb * gSpotLight.intensity * spotLightFactor * falloffFactor * spotLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
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