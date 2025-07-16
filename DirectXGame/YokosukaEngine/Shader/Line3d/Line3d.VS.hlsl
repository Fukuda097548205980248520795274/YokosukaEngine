#include "Line3d.hlsli"

// ���W�ϊ��̍s��
struct TransformationMatrix
{
    float4x4 worldViewProjection;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// CPU����̒��_���
struct VertexShaderInput
{
    // ���[�J�����W
    float4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.worldViewProjection);
    return output;
}