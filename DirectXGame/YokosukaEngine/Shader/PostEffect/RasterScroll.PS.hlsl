#include "Fullscreen.hlsli"

// ���X�^�[�X�N���[��
struct RasterScroll
{
    // �U��
    float amplitude;
    
    // ���g��
    float frequency;
    
    // �ʑ��I�t�Z�b�g
    float phaseOffset;
    
    // �X�N���[�����x
    float scrollSpeed;
    
    // ����
    float time;
};
ConstantBuffer<RasterScroll> gRasterScroll : register(b0);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // �X�N���[�����W��Y�������擾 (0-1�͈̔͂ɐ��K������Ă���ꍇ���l��)
    // �ʏ��SV_Position.y ��p���邪�A�����ł� input.uv.y ��Y���W�̑���Ƃ��Ďg����
    // ���ɂ����SV_Position.y ���g�����A���_�V�F�[�_�[�����Ԃ��ꂽ���炩��Y�l���g�����ς��
    float screenY = input.texcoord.y;

    // Y���W�Ɋ�Â����I�t�Z�b�g�ʂ��v�Z
    // Y���W�͈̔͂�0-1�̏ꍇ�Afrequency�̒������K�v
    float offsetX = gRasterScroll.amplitude *
    sin(screenY * gRasterScroll.frequency * 2 * 3.14159 + gRasterScroll.phaseOffset + gRasterScroll.time * gRasterScroll.scrollSpeed);

    // UV���W���I�t�Z�b�g
    float2 finalUV = input.texcoord;
    finalUV.x += offsetX; // X�����ɃI�t�Z�b�g��K�p

    // �I�t�Z�b�g���ꂽUV���W�Ńe�N�X�`�����T���v�����O
    output.color = gTexture.Sample(gSampler, finalUV);
    
    return output;
}