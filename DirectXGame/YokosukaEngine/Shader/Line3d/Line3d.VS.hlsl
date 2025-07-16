#include "Line3d.hlsli"

// 座標変換の行列
struct TransformationMatrix
{
    float4x4 worldViewProjection;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// CPUからの頂点情報
struct VertexShaderInput
{
    // ローカル座標
    float4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.worldViewProjection);
    return output;
}