#pragma once
#define NOMINMAX
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../Player/Player.h"

class Player
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="directionalLight"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d,const Vector3& position, const DirectionalLight* directionalLight);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドトランスフォームのインスタンスのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() const { return worldTransform_.get(); }

	/// <summary>
	/// 速度ベクトルのGetter
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() const { return velocity_; }


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



	// 加算するための加速度
	const float kAddAcceleration = 0.03f;

	// 速度減衰率
	const float kAttenuation = 0.3f;

	// 最大移動速度
	const float kMaxMoveSpeed = 0.3f;

	// 速度ベクトル
	Vector3 velocity_ = { 0.0f , 0.0f , 0.0f };


	/// <summary>
	/// 旋回制御
	/// </summary>
	void Turn();

	// 左右の向き
	enum class LRDirection
	{
		kRight,
		kLeft
	};

	// 左右の向き
	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回時間<秒>
	const float kTimeTurn = 0.3f;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	// 旋回タイマー
	float turnTimer_ = 0.0f;


	// 地面接地フラグ
	bool isGround_ = true;

	// 重力加速度
	const float kGravityAcceleration = 0.03f;

	// 最大落下速度
	const float kMaxFallSpeed = 1.0f;

	// ジャンプ初速
	const float kJumpStartAcceleration = 0.8f;
};
