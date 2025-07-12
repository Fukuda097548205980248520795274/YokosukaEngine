#include "Fullscreen.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    const float2 kIndex5x5[5][5] =
    {
        { { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f } },
        { { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
        { { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
        { { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
        { { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } }
    };
    
    float sum = 25.0f;
    
    const float kKernel5x5[5][5] =
    {
        { 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum },
        { 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum },
        { 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum },
        { 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum },
        { 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum, 1.0f / sum }
    };
    
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
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            // ���݂̃e�N�X�`�����W�n�����߂�
            float2 texcoord = input.texcoord + kIndex5x5[x][y] * uvStepSize;
            
            // �F�� 1.0f / 9.0f �|���đ���
            float3 feathColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += feathColor * kKernel5x5[x][y];
        }
    }
    
    return output;
}