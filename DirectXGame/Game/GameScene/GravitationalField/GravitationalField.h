#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class Planet;

class GravitationalField
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, float radius);

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
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// 当たり判定の球のGetter
	/// </summary>
	/// <returns></returns>
	Sphere GetCollisionSphere() const;

	/// <summary>
	/// 惑星のインスタンスのSetter
	/// </summary>
	/// <param name="planet"></param>
	void SetPlanetInstance(Planet* planet) { planet_ = planet; }

	/// <summary>
	/// 惑星のインスタンスのGetter
	/// </summary>
	/// <returns></returns>
	Planet* GetPlanetInstance() const { return planet_; }

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	void OnCollision();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// 惑星のインスタンス
	Planet* planet_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;


	// 半径
	float radius_ = 0.0f;
};

