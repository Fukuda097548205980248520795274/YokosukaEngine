
// �}�e���A��
struct Material
{
    float4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

// �s�N�Z���V�F�[�_�̏o��
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// main�֐�
PixelShaderOutput main()
{
    PixelShaderOutput output;
    output.color = gMaterial.color;
    return output;
}