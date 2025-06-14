#pragma once
#include <array>
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class DeathParticle
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DeathParticle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine , const Camera3D* camera3d, const Vector3& position);

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
	bool IsFinished() { return isFinished_;  }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;


	// パーティクルの数
	static inline const uint32_t kNumParticles = 8;

	// ワールドトランスフォーム
	std::array<WorldTransform*, kNumParticles> worldTransforms_;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// 色
	Vector4 color_ = { 1.0f , 1.0f , 1.0f , 1.0f };


	// 消滅のするまでの時間 <秒>
	const float kDuration = 2.0f;

	// 移動の速さ
	const float kSpeed = 0.05f;

	// 分割した1つ分の角度
	const float kAngleUnit = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kNumParticles);


	// 終了フラグ
	bool isFinished_ = false;

	// 経過時間カウント
	float counter_ = 0.0f;
};

