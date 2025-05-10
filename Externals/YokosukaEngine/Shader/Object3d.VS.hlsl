
// 頂点シェーダの出力
struct VertexShaderOutput
{
    // 同時クリップ空間
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
    output.position = input.position;
    return output;
}