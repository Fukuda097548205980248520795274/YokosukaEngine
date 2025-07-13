#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <stdint.h>

#include "../../ParticleEffect/Particle/Particle.h"
#include "../../ParticleEffect/Emitter/Emitter.h"
#include "../../ParticleEffect/Feild/Feild.h"

// タイプ
enum ParticleType
{
	kModel,
	kBillboard,
};

class ParticleStore
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 放出処理
	/// </summary>
	/// <param name="particleHandle"></param>
	void Emit(uint32_t particleHandle);

	/// <summary>
	/// パーティクルを読み込む
	/// </summary>
	/// <param name="particle"></param>
	uint32_t LoadParticle(Particle* particle);


private:


	// 格納できるパーティクル数
	const uint32_t kMaxNumParticles = 512;

	// 使用したパーティクルデータ数
	uint32_t useNumParticles_ = 0;

	// パーティクルの格納データ
	struct ParticleData
	{
		// パーティクルの数
		uint32_t numMaxInstance = 0;

		// インスタンシングリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource = nullptr;

		// マテリアルリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;

		// パーティクル
		Particle* state = nullptr;

		// タイプ
		Type type;
	};

	// パーティクルの格納データ
	ParticleData particleData_[512];
};

