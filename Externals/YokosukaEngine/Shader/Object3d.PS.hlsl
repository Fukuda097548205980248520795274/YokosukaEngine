#include "Object3d.hlsli"

// �}�e���A��
struct Material
{
    float4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

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
    
    // �e�N�X�`���ƐF����Z����
    output.color = gMaterial.color * textureColor;
    
    return output;
}