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
};
StructuredBuffer<DirectionalLight> gDirectionalLight : register(t1);

// �g�p���Ă��镽�s�����̐�
struct UseNumDirectionalLight
{
    uint num;
};
ConstantBuffer<UseNumDirectionalLight> gUseNumDirectionalLight : register(b1);


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

// �X�|�b�g���C�g
struct SpotLight
{
    // �F
    float4 color;
    
    // �ʒu
    float3 position;
    
    // �P�x
    float intensity;
    
    // ����
    float3 direction;
    
    // ���C�g�̓͂��ő勗��
    float distance;
    
    // ������
    float decay;
    
    // �X�|�b�g���C�g�̗]��
    float cosAngle;
    
    // �t�H�[���I�t�̊J�n�l
    float fallofStart;
};
ConstantBuffer<SpotLight> gSpotLight : register(b4);

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
        
        // �g�U���˂̐F
        float3 diffuseDirectionalLightColor = { 0.0f, 0.0f, 0.0f };
        
        // ���ʔ��˂̐F
        float3 speculerDirectionalLightColor = { 0.0f, 0.0f, 0.0f };
        
        for (uint i = 0; i < gUseNumDirectionalLight.num; i++)
        {
            // ���ˌ�
            float directionalLightNdotL = dot(normalize(input.normal), -gDirectionalLight[i].direction);
            float directionalLightCos = pow(directionalLightNdotL * 0.5f + 0.5f, 2.0f);
        
            // ���ˌ��̔��˃x�N�g��
            float3 directionalLightHalfVector = normalize(-gDirectionalLight[i].direction + toEye);
            float directionalLightNDotH = dot(normalize(input.normal), directionalLightHalfVector);
            float directionalLightSpeculerPow = pow(saturate(directionalLightNDotH), gMaterial.shininesse);
        
            diffuseDirectionalLightColor += gDirectionalLight[i].color.rgb * directionalLightCos;
            speculerDirectionalLightColor += gDirectionalLight[i].color.rgb * directionalLightSpeculerPow;
        }
        
         // �g�U����
        float3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * diffuseDirectionalLightColor;
        
        // ���ʔ���
        float3 speculerDirectionalLight = speculerDirectionalLightColor * float3(1.0f, 1.0f, 1.0f);
        
        
        /*-----------
            �_����
        -----------*/
        
        // �_�����ւ̋���
        float distance = length(gPointLight.position - input.worldPosition);
        
        // �t��摥�ɂ�錸���W��
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        
        // ���ˌ�
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        float pointLightNdotL = dot(normalize(input.normal), -pointLightDirection);
        float pointLightCos = pow(pointLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // ���ˌ��̔��˃x�N�g��
        float3 pointLightHalfVector = normalize(-pointLightDirection + toEye);
        float pointLightNdotH = dot(normalize(input.normal), pointLightHalfVector);
        float pointLightSpeculerPow = pow(saturate(pointLightNdotH), gMaterial.shininesse);
        
        // �g�U����
        float3 diffusePointLight =
        gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * pointLightCos * gPointLight.intensity * factor;
        
        // ���ʔ���
        float3 speculerPointLight =
        gPointLight.color.rgb * gPointLight.intensity * factor * pointLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        /*------------------
            �X�|�b�g���C�g
        ------------------*/
        
        // �X�|�b�g���C�g�ւ̋���
        float spotLightDistance = length(gSpotLight.position - input.worldPosition);
        
        // �t��摥�ɂ�錸���W��
        float spotLightFactor = pow(saturate(-spotLightDistance / gSpotLight.distance + 1.0f), gSpotLight.decay);
        
        // ���ˌ������߂�
        float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        float spotLightNdotL = dot(normalize(input.normal), -spotLightDirectionOnSurface);
        float spotLightCos = pow(spotLightNdotL * 0.5f + 0.5f, 2.0f);
        
        // ���ˌ��̔��˃x�N�g��
        float3 spotLightHalfVector = normalize(-spotLightDirectionOnSurface + toEye);
        float spotLightNdotH = dot(normalize(input.normal), spotLightHalfVector);
        float spotLightSpeculerPow = pow(saturate(spotLightNdotH), gMaterial.shininesse);
        
        // �t�H�[���I�t
        float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.fallofStart - gSpotLight.cosAngle));
        
        // �g�U����
        float3 diffuseSpotLight =
        gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * spotLightCos * gSpotLight.intensity * spotLightFactor * falloffFactor;
        
        // ���ʔ���
        float3 speculerSpotLight =
        gPointLight.color.rgb * gSpotLight.intensity * spotLightFactor * falloffFactor * spotLightSpeculerPow * float3(1.0f, 1.0f, 1.0f);
        
        
        // �S�ď�Z����
        output.color.rgb = 
        diffuseDirectionalLight + speculerDirectionalLight + diffusePointLight + speculerPointLight + diffuseSpotLight + speculerSpotLight;
        
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