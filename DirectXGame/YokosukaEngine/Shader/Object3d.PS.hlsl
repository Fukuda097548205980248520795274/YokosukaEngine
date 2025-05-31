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

// �_����
struct PointLight
{
    // �F
    float4 color;
    
    // �ʒu
    float3 position;
    
    // �P�x
    float intensity;
    
    // ���C�g�̓͂��ő勗��
    float radius;
    
    // ������
    float decay;
};
ConstantBuffer<PointLight> gPointLight : register(b3);

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
        // �J�����ւ̕���
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
        
        /*-------------
            ���s����
        -------------*/
        
        // ���s�����̓��ˌ�
        float directionalLightNdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // ���s�����̓��ˌ��̔��˃x�N�g��
        float3 directionalLightHalfVector = normalize(-gDirectionalLight.direction + toEye);
        float directionalLightNDotH = dot(normalize(input.normal), directionalLightHalfVector);
        float directionalLightSpeculerPow = pow(saturate(directionalLightNDotH), gMaterial.shininesse);
        
         // �g�U���ˁi���s�����j
        float3 diffuseDirectionalLight =
        gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intensity;
        
        // ���ʔ��ˁi���s�����j
        float3 speculerDirectionalLight =
        gDirectionalLight.color.rgb * gDirectionalLight.intensity * directionalLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        /*-----------
            �_����
        -----------*/
        
        // �_�����ւ̋���
        float distance = length(gPointLight.position - input.worldPosition);
        
        // �t��摥�ɂ�錸���W��
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        
        // �_�����̓��ˌ�
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        float pointLightNdotL = dot(normalize(input.normal), -pointLightDirection);
        float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // �_�����̓��ˌ��̔��˃x�N�g��
        float3 pointLightHalfVector = normalize(-pointLightDirection + toEye);
        float pointLightNdotH = dot(normalize(input.normal), pointLightHalfVector);
        float pointLightSpeculerPow = pow(saturate(pointLightNdotH), gMaterial.shininesse);
        
        // �g�U���ˁi�_�����j
        float3 diffusePointLight =
        gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * pointLightCos * gPointLight.intensity * factor;
        
        // ���ʔ��ˁi�_�����j
        float3 speculerPointLight =
        gPointLight.color.rgb * gPointLight.intensity * factor * pointLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        
        // �S�ď�Z����
        output.color.rgb = diffuseDirectionalLight + speculerDirectionalLight + diffusePointLight + speculerPointLight;
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