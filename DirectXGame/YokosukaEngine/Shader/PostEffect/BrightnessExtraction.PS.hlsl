#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Luminance
{
    // �P�x��臒l
    float threshold;
};
ConstantBuffer<Luminance> gLuminance : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // 1. �e�N�X�`�����猻�݂̃s�N�Z���̐F���T���v�����O
    output.color = gTexture.Sample(gSampler, input.texcoord);

    // 2. RGB�J���[����P�x�i���邳�j���v�Z����
    // ��ʓI�ȕ��@�Ƃ��āANTSC�W���iRec. 709�̋P�x�v�Z�j���g�p���܂��B
    // R * 0.299 + G * 0.587 + B * 0.114
    // �A���t�@�`�����l���͋P�x�v�Z�ɂ͊܂߂܂���B
    float luminance = dot(output.color.rgb, float3(0.299, 0.587, 0.114));
    // �܂��́A�����ƒP���ɍő�R���|�[�l���g���g�����@������܂����ANTSC�W���̕������o�I�Ɏ��R�Ȃ��Ƃ������ł��B
    // float luminance = max(max(color.r, color.g), color.b);

    // 3. �P�x��臒l�𒴂��Ă��邩���肷��
    if (luminance <= gLuminance.threshold)
    {
        // 臒l�ȉ��̏ꍇ�́A���S�ɍ����o�� (�A���t�@�͒ʏ�1.0)
        output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    
    return output;
}