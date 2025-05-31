#include "Object3d.hlsli"

// �}�e���A��
struct Material
{
    // �F
    float4 color;
    
    // ���C�e�B���O�L����
    int enableLighting;
    
    // UV�g�����X�t�H�[��
    float4x4 uvTransform;
    
    // ����x
    float shininesse;
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

// �J����
struct Camera
{
    float3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b2);

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
    
    // ���W�ϊ������e�N�X�`���@�i�e�N�X�`�����W�n��4�����ɕϊ�����j
    float4 transfomedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    
    // ���W�ϊ������e�N�X�`�����W�n�ŁA�e�N�X�`���̐F���擾����
    float4 textureColor = gTexture.Sample(gSampler, transfomedUV.xy);
    
    // �e�N�X�`���̐F�̃��l��0.0f�̂Ƃ���Pixel�����p����
    if (textureColor.a == 0.0f)
    {
        discard;
    }
    
    // ���C�e�B���O�K�p
    if (gMaterial.enableLighting)
    {
        // �p�x�Ō������閾�邳�����߂� 0 ~ 1
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // �J�����ւ̕���
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
        // ���ˌ��̔��˃x�N�g��
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float speculerPow = pow(saturate(NDotH), gMaterial.shininesse);
        
        // �g�U����
        float3 diffuse = 
        gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        // ���ʔ���
        float3 speculer = 
        gDirectionalLight.color.rgb * gDirectionalLight.intensity * speculerPow * float3(1.0f, 1.0f, 1.0f);
        
        // �S�ď�Z����
        output.color.rgb = diffuse + speculer;
        output.color.a = gMaterial.color.a * textureColor.a;

    }
    else
    {
        // ���C�f�B���O����
        
        // �e�N�X�`���ƐF����Z����
        output.color = gMaterial.color * textureColor;
    }
    
    // �o�͂���F�̃��l��0.0f�̂Ƃ���Pixel�����p����
    if (output.color.a == 0.0f)
    {
        discard;
    }
    
    return output;
}