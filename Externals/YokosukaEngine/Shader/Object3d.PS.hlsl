
// マテリアル
struct Material
{
    float4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

// ピクセルシェーダの出力
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// main関数
PixelShaderOutput main()
{
    PixelShaderOutput output;
    output.color = gMaterial.color;
    return output;
}