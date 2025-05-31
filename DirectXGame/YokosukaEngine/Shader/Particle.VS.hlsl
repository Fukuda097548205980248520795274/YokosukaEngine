#include "Particle.hlsli"

// パーティクル
struct ParticleForGPU
{
    float4x4 worldViewProjection;
    float4x4 world;
    float4 color;
};
StructuredBuffer<ParticleForGPU> gParticle : register(t0);

// 入力された値
struct VertexShaderInput
{
    // ローカル座標
    float4 position : POSITION0;
    
    // テクスチャ座標
    float2 texcoord : TEXCOORD0;
    
    // 頂点法線
    float3 normal : NORMAL0;
};

// main関数
VertexShaderOutput main(VertexShaderInput input , uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    
    // 同次クリップ空間に変換する
    output.position = mul(input.position, gParticle[instanceId].worldViewProjection);
    output.texcoord = input.texcoord;
    output.color = gParticle[instanceId].color;
    
    return output;
}