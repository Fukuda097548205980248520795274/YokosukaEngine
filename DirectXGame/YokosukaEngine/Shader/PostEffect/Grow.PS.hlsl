#include "Fullscreen.hlsli"

struct Grow
{
    float bloomThreshold;
    float bloomIntensity;
};
ConstantBuffer<Grow> gGrow : register(b0);

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // ���̓e�N�X�`�����猻�݂̃s�N�Z���̐F���擾
    float4 texColor = gTexture.Sample(gSampler, input.texcoord);

    // �P�x���v�Z (Rec.709�̋P�x�v�Z��)
    // RGB���ꂼ��̊�^�x��ς��邱�ƂŁA�l�Ԃ̖ڂ̖��邳�̒m�o�ɋ߂��P�x��������
    float luminance = dot(texColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));

    // �P�x��臒l�𒴂��Ă��邩�`�F�b�N
    if (luminance > gGrow.bloomThreshold)
    {
        // 臒l�𒴂��Ă���ꍇ�A���̐F���o��
        // �P�x�Ɋ�Â��ĐF����Z���邱�ƂŁA��薾�邢�������������邱�Ƃ��\
        // return color * (luminance - g_bloomThreshold); // �I�v�V�����F臒l����̍����ŋ��x�𒲐�
        output.color = texColor;
    }
    else
    {
        // 臒l�ȉ��̏ꍇ�A�����o�� (�O���[�̌��ɂȂ�Ȃ�)
        output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    return output;
}