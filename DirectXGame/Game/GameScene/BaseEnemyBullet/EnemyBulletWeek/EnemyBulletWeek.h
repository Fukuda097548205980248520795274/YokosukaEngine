#pragma once
#include "../BaseEnemyBullet.h"

class EnemyBulletWeek : public BaseEnemyBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, WorldTransform* parent) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 向きのSetter
	/// </summary>
	/// <param name="direction"></param>
	void SetDirection(const Vector3& direction) { direction_ = Normalize(direction); }

	/// <summary>
	/// 本体のワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetBulletWorldPosition() const;


private:


	// 向き
	Vector3 direction_ = { 0.0f , 0.0f , 0.0f };

	// 移動速度
	const float speed = 0.5f;


	// 寿命
	const float kLifeTime = 4.0f;

	// 寿命タイマー
	float lifeTimer_ = 0.0f;


	/*----------
	    本体
	----------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> bulletWorldTransform_ = nullptr;

	// モデルハンドル
	uint32_t bulletModelHandle_ = 0;

	// 弾のポイントライト
	std::unique_ptr<PointLight> bulletPointLight_ = nullptr;
};

