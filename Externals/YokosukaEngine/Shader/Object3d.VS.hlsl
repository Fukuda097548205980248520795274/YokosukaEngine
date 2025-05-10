
// 座標変換の行列
struct TransformationMatrix
{
    float4x4 worldViewProjection;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// 頂点シェーダの出力
struct VertexShaderOutput
{
    // 同次クリップ空間
    float4 position : SV_POSITION;
};

// 入力された値
struct VertexShaderInput
{
    // ローカル座標
    float4 position : POSITION0;
};

// main関数
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // 同次クリップ空間に変換する
    output.position = mul(input.position, gTransformationMatrix.worldViewProjection);
    return output;
}