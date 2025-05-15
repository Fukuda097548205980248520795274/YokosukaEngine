#pragma once
#include "../../../../../YokosukaEngine/Include/YokosukaEngine.h"

class Particle
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera"></param>
	/// <param name="worldPosition"></param>
	void Initialize(YokosukaEngine* engine, Camera3D* camera, const Engine::Vector3& worldPosition,uint32_t* textureHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	int IsFinished() { return isFinished_; }


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
	Engine::Vector4 color_ = { 1.0f , 0.7f , 0.0f ,1.0f };


	// 寿命
	const float kLifeTime = 0.6f;

	// デスタイマー
	float deathTimer_ = 0.0f;

	// 終了フラグ
	int isFinished_ = false;
};

