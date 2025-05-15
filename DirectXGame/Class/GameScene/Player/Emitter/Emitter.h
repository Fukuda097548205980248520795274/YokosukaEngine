#pragma once
#include "../../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Particle/Particle.h"

class Emitter
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Emitter();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(YokosukaEngine* engine, Camera3D* camera, WorldTransform* parent, const Engine::Vector3& position, uint32_t* textureHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Engine::Vector3 GetWorldPosition();


private:

	// エンジン
	YokosukaEngine* engine_ = nullptr;

	// カメラ
	Camera3D* camera_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform* worldTransform_ = nullptr;

	// UVトランスフォーム
	WorldTransform* uvTransform_ = nullptr;

	// テクスチャ
	uint32_t* textureHandle_ = nullptr;

	// 色
	Engine::Vector4 color = { 1.0f , 1.0f , 1.0f , 1.0f };


	// パーティクルのリスト
	std::list<Particle*> particles_;

	
	// 放出時間
	const float kEmissionTime = 0.05f;

	// 放出の復活処理
	float emissionRespawnTimer = 0.0f;
};

