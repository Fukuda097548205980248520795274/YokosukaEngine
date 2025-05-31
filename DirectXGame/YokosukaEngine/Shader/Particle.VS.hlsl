#include "Particle.hlsli"

// �p�[�e�B�N��
struct ParticleForGPU
{
    float4x4 worldViewProjection;
    float4x4 world;
    float4 color;
};
StructuredBuffer<ParticleForGPU> gParticle : register(t0);

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
    output.position = mul(input.position, gParticle[instanceId].worldViewProjection);
    output.texcoord = input.texcoord;
    output.color = gParticle[instanceId].color;
    
    return output;
}