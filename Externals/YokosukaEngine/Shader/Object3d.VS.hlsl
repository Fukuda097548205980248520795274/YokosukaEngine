#include "Object3d.hlsli"

// 座標変換の行列
struct TransformationMatrix
{
    float4x4 worldViewProjection;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// 入力された値
struct VertexShaderInput
{
    // ローカル座標
    float4 position : POSITION0;
    
    // テクスチャ座標
    float2 texcoord : TEXCOORD0;
};

// main関数
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // 同次クリップ空間に変換する
    output.position = mul(input.position, gTransformationMatrix.worldViewProjection);
    output.texcoord = input.texcoord;
    
    return output;
}