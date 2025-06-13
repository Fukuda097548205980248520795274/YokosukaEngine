#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class Player;

class Enemy
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="directionalLight"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールド座標を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// AABBを取得する
	/// </summary>
	/// <returns></returns>
	AABB GetAABB() const;

	/// <summary>
	/// 衝突応答処理
	/// </summary>
	/// <param name="player"></param>
	void OnCollision(const Player* player);


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 平行光源
	const DirectionalLight* directionalLight_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;

	// スポットライト
	std::unique_ptr<SpotLight> spotLight_ = nullptr;


	// 歩行の速さ
	const float kWalkSpeed = 0.03f;

	// 速度ベクトル
	Vector3 velocity_ = { 0.0f , 0.0f , 0.0f };


	// 最初の角度 [度]
	const float kWalkMotionAngleStart = -30.0f;

	// 最後の角度 [度]
	const float kWalkMotionAngleEnd = 30.0f;

	// 歩きモーションの周期となる時間 [秒]
	const float kWalkMotionTime = 2.0f;

	// 歩きモーションの経過時間
	float walkMotionTimer_ = 0.0f;
};

