#include "Object3d.hlsli"

// ���W�ϊ��̍s��
struct TransformationMatrix
{
    float4x4 worldViewProjection;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// ���͂��ꂽ�l
struct VertexShaderInput
{
    // ���[�J�����W
    float4 position : POSITION0;
    
    // �e�N�X�`�����W
    float2 texcoord : TEXCOORD0;
};

// main�֐�
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // �����N���b�v��Ԃɕϊ�����
    output.position = mul(input.position, gTransformationMatrix.worldViewProjection);
    output.texcoord = input.texcoord;
    
    return output;
}