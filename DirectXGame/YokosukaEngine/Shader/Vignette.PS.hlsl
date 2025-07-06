#include "FullScreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D gTexture : register(t0);
SamplerState gSample : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // �e�N�X�`���̐F�f�[�^��n��
    output.color = gTexture.Sample(gSample, input.texcoord);
    
    // ���͂�0.0f�@���S�ɂȂ�قǖ��邭�Ȃ�悤�ɂ���
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    
    // correct�����ł͒��S�̍ő�l���Ⴂ�̂Œ�������
    float vignette = correct.x * correct.y * 16.0f;
    
    // �Ƃ肠����0.8�悷��
    vignette = saturate(pow(vignette, 0.8f));
    
    // �W���Ƃ��ď�Z
    output.color.rgb *= vignette;
    
    
    return output;
}