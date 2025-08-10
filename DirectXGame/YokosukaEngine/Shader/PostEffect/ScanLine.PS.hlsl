#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct ScanlineParams
{
    // ��ʃT�C�Y
    float2 screenSize;
    
    // �������̑���
    float thickness;
    
    // �������̔Z�� (0.0f ~ 1.0f)
    float intensity;
    
    // �������̊Ԋu
    float spacing;
    
    // ����
    float time;
    
    // �ړ����x
    float speed;
};
ConstantBuffer<ScanlineParams> gScanlineParams : register(b0);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    // �o��
    PixelShaderOutput output;
    
    float4 originalColor = gTexture.Sample(gSampler, input.texcoord);

    // �s�N�Z���P�ʂ�Y���W���v�Z
    float pixelY = input.texcoord.y * gScanlineParams.screenSize.y;

    // ���ԂƑ��x�Ɋ�Â��āA�������̃I�t�Z�b�g���v�Z
    // _Time�̓Q�[���̌o�ߎ��ԁi�b�P�ʂȂǁj
    // _Speed�͑������̈ړ����x
    float yOffset = fmod(gScanlineParams.time * gScanlineParams.speed, gScanlineParams.spacing);

    // Y���W����I�t�Z�b�g�������āA�������̈ʒu�����炷
    float yWithOffset = pixelY - yOffset;

    // �������̔���ɃI�t�Z�b�g�t����Y���W���g�p
    if (fmod(yWithOffset, gScanlineParams.spacing) < gScanlineParams.thickness)
    {
        output.color = originalColor * (1.0 - gScanlineParams.intensity);
    }
    else
    {
        output.color = originalColor;
    }
    
    return output;
}