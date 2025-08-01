#include "Primitive.hlsli"

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
};
StructuredBuffer<DirectionalLight> gDirectionalLight : register(t1);

// 使用している平行光源の数
struct UseNumDirectionalLight
{
    uint num;
};
ConstantBuffer<UseNumDirectionalLight> gUseNumDirectionalLight : register(b1);


// カメラ
struct Camera
{
    float3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b2);


// ポイントライト
struct PointLight
{
    // 色
    float4 color;
    
    // 位置
    float3 position;
    
    // ライトの届く最大距離
    float radius;
    
    // 減衰率
    float decay;
};
StructuredBuffer<PointLight> gPointLight : register(t2);

// 使用しているポイントライトの数
struct UseNumPointLight
{
    uint num;
};
ConstantBuffer<UseNumPointLight> gUseNumPointLight : register(b3);

// スポットライト
struct SpotLight
{
    // 色
    float4 color;
    
    // 位置
    float3 position;
    
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
StructuredBuffer<SpotLight> gSpotLight : register(t3);

// 使用しているスポットライトの数
struct UseNumSpotLight
{
    uint num;
};
ConstantBuffer<UseNumSpotLight> gUseNumSpotLight : register(b4);

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
        
        // 拡散反射の色
        float3 diffuseDirectionalLightColor = { 0.0f, 0.0f, 0.0f };
        
        // 鏡面反射の色
        float3 speculerDirectionalLightColor = { 0.0f, 0.0f, 0.0f };
        
        for (uint i = 0; i < gUseNumDirectionalLight.num; i++)
        {
            // 入射光
            float directionalLightNdotL = dot(normalize(input.normal), -gDirectionalLight[i].direction);
            float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
        
            // 入射光の反射ベクトル
            float3 directionalLightHalfVector = normalize(-gDirectionalLight[i].direction + toEye);
            float directionalLightNDotH = dot(normalize(input.normal), directionalLightHalfVector);
            float directionalLightSpeculerPow = pow(saturate(directionalLightNDotH), gMaterial.shininesse);
        
            diffuseDirectionalLightColor += gDirectionalLight[i].color.rgb * directionalLightCos;
            speculerDirectionalLightColor += gDirectionalLight[i].color.rgb * directionalLightSpeculerPow;
        }
        
         // 拡散反射
        float3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * diffuseDirectionalLightColor;
        
        // 鏡面反射
        float3 speculerDirectionalLight = speculerDirectionalLightColor * float3(1.0f, 1.0f, 1.0f);
        
        
        /*-----------
            点光源
        -----------*/

        // 拡散反射の色
        float3 diffusePointLightColor = { 0.0f, 0.0f, 0.0f };

         // 鏡面反射
        float3 speculerPointLightColor = { 0.0f, 0.0f, 0.0f };

        for (uint i = 0; i < gUseNumPointLight.num; i++)
        {
            // 点光源への距離
            float distance = length(gPointLight[i].position - input.worldPosition);

            // 逆二乗則による減速係数
            float factor = pow(saturate(-distance / gPointLight[i].radius + 1.0f), gPointLight[i].decay);

            // 入射光
            float3 pointLightDirection = normalize(input.worldPosition - gPointLight[i].position);
            float pointLightNdotL = dot(normalize(input.normal), -pointLightDirection);
            float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);

            // 入射光の反射ベクトル
            float3 pointLightHalfVector = normalize(-pointLightDirection + toEye);
            float pointLightNdotH = dot(normalize(input.normal), pointLightHalfVector);
            float pointLightSpeculerPow = pow(saturate(pointLightNdotH), gMaterial.shininesse);
    
            // 各ライトの寄与を加算
            diffusePointLightColor += gPointLight[i].color.rgb * (pointLightCos * factor);
            speculerPointLightColor += gPointLight[i].color.rgb * (factor * pointLightSpeculerPow);
        }

        // 拡散反射
        float3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * diffusePointLightColor;

        // 鏡面反射
        float3 speculerPointLight = speculerPointLightColor * float3(1.0f, 1.0f, 1.0f);
        
        
        /*------------------
            スポットライト
        ------------------*/
        
        // 拡散反射の色
        float3 diffuseSpotLightColor = { 0.0f, 0.0f, 0.0f };
        
        // 鏡面反射の色
        float3 speculerSpotLightColor = { 0.0f, 0.0f, 0.0f };
        
        for (uint i = 0; i < gUseNumSpotLight.num; ++i)
        {
            // スポットライトへの距離
            float spotLightDistance = length(gSpotLight[i].position - input.worldPosition);
        
            // 逆二乗則による減衰係数
            float spotLightFactor = pow(saturate(-spotLightDistance / gSpotLight[i].distance + 1.0f), gSpotLight[i].decay);
        
            // 入射光を求める
            float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight[i].position);
            float spotLightNdotL = dot(normalize(input.normal), -spotLightDirectionOnSurface);
            float spotLightCos = pow(spotLightNdotL * 0.5f + 0.5f, 2.0f);
        
            // 入射光の反射ベクトル
            float3 spotLightHalfVector = normalize(-spotLightDirectionOnSurface + toEye);
            float spotLightNdotH = dot(normalize(input.normal), spotLightHalfVector);
            float spotLightSpeculerPow = pow(saturate(spotLightNdotH), gMaterial.shininesse);
        
            // フォールオフ
            float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight[i].direction);
            float falloffFactor = saturate((cosAngle - gSpotLight[i].cosAngle) / (gSpotLight[i].fallofStart - gSpotLight[i].cosAngle));
            
            diffuseSpotLightColor += gSpotLight[i].color.rgb * spotLightCos * spotLightFactor * falloffFactor;
            speculerSpotLightColor += gSpotLight[i].color.rgb * spotLightFactor * falloffFactor * spotLightSpeculerPow;

        }
        
        // 拡散反射
        float3 diffuseSpotLight = gMaterial.color.rgb * textureColor.rgb * diffuseSpotLightColor;
        
        // 鏡面反射
        float3 speculerSpotLight = speculerSpotLightColor * float3(1.0f, 1.0f, 1.0f);
        
        
        
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