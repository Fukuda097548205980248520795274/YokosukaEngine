#include "Object3d.hlsli"

// 座標変換の行列
struct TransformationMatrix
{
    float4x4 worldViewProjection;
    float4x4 world;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// 入力された値
struct VertexShaderInput
{
    // ローカル座標
    float4 position : POSITION0;
    
    // テクスチャ座標
    float2 texcoord : TEXCOORD0;
    
    // 頂点法線
    float3 normal : NORMAL0;
};

// main関数
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // 同次クリップ空間に変換する
    output.position = mul(input.position, gTransformationMatrix.worldViewProjection);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.world));
    
    return output;
}