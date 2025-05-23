#include "Particle.hlsli"

// ���W�ϊ��̍s��
struct TransformationMatrix
{
    float4x4 worldViewProjection;
    float4x4 world;
};
StructuredBuffer<TransformationMatrix> gTransformationMatrices : register(t0);

// ���͂��ꂽ�l
struct VertexShaderInput
{
    // ���[�J�����W
    float4 position : POSITION0;
    
    // �e�N�X�`�����W
    float2 texcoord : TEXCOORD0;
    
    // ���_�@��
    float3 normal : NORMAL0;
};

// main�֐�
VertexShaderOutput main(VertexShaderInput input , uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    
    // �����N���b�v��Ԃɕϊ�����
    output.position = mul(input.position, gTransformationMatrices[instanceId].worldViewProjection);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrices[instanceId].world));
    
    return output;
}