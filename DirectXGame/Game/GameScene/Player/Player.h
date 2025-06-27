#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class Planet;
class GravitationalField;

class Player
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
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 当たり判定の球のGetter
	/// </summary>
	/// <returns></returns>
	Sphere GetCollisionSphere();

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="planet"></param>
	void OnCollision(const Planet* planet);

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	void OnCollision(const GravitationalField* gravitationalField);


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;


	// 半径
	float radius_ = 1.0f;


	// 重力に引っ張られているかどうか
	bool isGravityPull_ = false;

	// 着地したかどうか
	bool isGround_ = false;

	// 重力場の中心方向ベクトル
	Vector3 toGravity_ = { 0.0f , 0.0f , 0.0f };

	// 惑星の中心方向ベクトル
	Vector3 toPlanet_ = { 0.0f , 0.0f , 0.0f };
};

