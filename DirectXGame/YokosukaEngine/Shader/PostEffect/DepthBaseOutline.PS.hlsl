#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

Texture2D<float> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);

static const float2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f, }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f, }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f, }, { 1.0f, 1.0f } }
};

static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f }
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }
};

// �P�x��ϊ�����
float Luminance(float3 v)
{
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));

}

PixelShaderOutput main(VertexShaderOutput input)
{
    /*-------------------------
       uvStepSize���Z�o���� 
   -------------------------*/
    
    // Texture�̏c���̕�
    uint width, height;
    gTexture.GetDimensions(width, height);
    
    // �t�������߂�
    float2 uvStepSize = float2(rcp(float(width)), rcp(float(height)));
    
    
    /*-------------
        ��ݍ���
    -------------*/
    
    // �c�����ꂼ��̏�ݍ��݂̌��ʂ��i�[����
    float2 difference = float2(0.0f, 0.0f);
    
    // �F���P�x�ɕϊ����ď�ݍ��݂��s��
    for (uint x = 0; x < 3; x++)
    {
        for (uint y = 0; y < 3; y++)
        {
            float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            float luminance = Luminance(fetchColor);

            difference.x += luminance * kPrewittHorizontalKernel[x][y];
            difference.y += luminance * kPrewittVerticalKernel[x][y];
        }
    }
    
    
    /*------------------
        ���ʂ��o�͂���
    ------------------*/
    
    // �ω��̒������E�F�C�g�ɍ�������
    float weight = length(difference);
    
    // 0 ~ 1�ɂ���
    weight = saturate(weight * 6.0f);
    
    
    PixelShaderOutput output;
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;
    output.color.a = 1.0f;
    return output;
}