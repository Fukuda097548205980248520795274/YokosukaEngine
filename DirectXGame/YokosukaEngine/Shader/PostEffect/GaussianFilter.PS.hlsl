#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const float PI = 3.14159265f;

float gauss(float x, float y , float sigma)
{
    float exponent = (-x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponent) * rcp(denominator);

}


PixelShaderOutput main(VertexShaderOutput input)
{
    float2 index3x3[5][5] =
    {
        { { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, {2.0f , -2.0f } },
        { { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
        { { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
        { { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
        { { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } }
    };
    
    
    /*-------------------
        �J�[�l�������߂�
    -------------------*/
    
    float weight = 0.0f;
    float kernel3x3[5][5];
    for (uint x = 0; x < 5; x++)
    {
        for (uint y = 0; y < 5; y++)
        {
            kernel3x3[x][y] = gauss(index3x3[x][y].x, index3x3[x][y].y, 2.0f);
            weight += kernel3x3[x][y];
        }

    }
    
    
   /*-------------------------
       uvStepSize���Z�o���� 
   -------------------------*/
    
    // Texture�̏c���̕�
    uint width, height;
    gTexture.GetDimensions(width, height);
    
    // �t�������߂�
    float2 uvStepSize = float2(rcp(float(width)), rcp(float(height)));
    
    
    PixelShaderOutput output;
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    
    // 3x3���[�v����
    for (uint x = 0; x < 5; x++)
    {
        for (uint y = 0; y < 5; y++)
        {
            // ���݂̃e�N�X�`�����W�n�����߂�
            float2 texcoord = input.texcoord + index3x3[x][y] * uvStepSize;
            
            // �F�� 1.0f / 9.0f �|���đ���
            float3 feathColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += feathColor * kernel3x3[x][y];
        }
    }
    
    output.color.rgb *= rcp(weight);
    
    return output;
}