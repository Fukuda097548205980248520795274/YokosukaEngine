#include "Object3d.hlsli"

// �}�e���A��
struct Material
{
    // �F
    float4 color;
    
    // ���C�e�B���O�L����
    int enableLighting;
};
ConstantBuffer<Material> gMaterial : register(b0);

// ���s����
struct DirectionalLight
{
    // �F
    float4 color;
    
    // ����
    float3 direction;
    
    // �P�x
    float intensity;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

// �e�N�X�`���̐錾
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// �s�N�Z���V�F�[�_�̏o��
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// main�֐�
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // �e�N�X�`���̐F���擾����
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    // ���C�e�B���O�K�p
    if (gMaterial.enableLighting)
    {
        // �p�x�Ō������閾�邳�����߂� 0 ~ 1
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // �S�ď�Z����
        output.color = textureColor * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;

    }
    else
    {
        // ���C�f�B���O����
        
         // �e�N�X�`���ƐF����Z����
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}