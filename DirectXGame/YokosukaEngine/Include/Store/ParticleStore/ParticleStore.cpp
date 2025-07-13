#include "ParticleStore.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")

/// <summary>
/// 初期化
/// </summary>
void ParticleStore::Initialize()
{

}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="commandList"></param>
void ParticleStore::Draw(ID3D12GraphicsCommandList* commandList)
{

}

/// <summary>
/// 放出処理
/// </summary>
/// <param name="particleHandle"></param>
void ParticleStore::Emit(uint32_t particleHandle)
{

}

/// <summary>
/// パーティクルを読み込む
/// </summary>
/// <param name="particle"></param>
uint32_t ParticleStore::LoadParticle(Particle* particle)
{
	// nullptrチェック
	assert(particle);

	// 使用済みのパーティクルのステートは格納しない
	for (uint32_t i = 0; i < useNumParticles_; i++)
	{
		if (particle == particleData_[i].state)
			return;
	}

	// ステートを受け取る
	particleData_[useNumParticles_].state = particle;

	// カウントする
	useNumParticles_++;
}