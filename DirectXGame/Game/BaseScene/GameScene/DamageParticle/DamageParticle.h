#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class DamageParticle
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position);

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
	bool IsFinished()const { return isFinished_; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_ = 0;


	// タイマー
	float timer_ = 0.0f;

	// 最大時間
	float maxTime_ = 0.1f;

	// 終了フラグ
	bool isFinished_ = false;
};

