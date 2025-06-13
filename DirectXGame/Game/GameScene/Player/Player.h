#pragma once
#define NOMINMAX
#include <array>
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../Player/Player.h"

// 前方宣言
class MapChipField;
class Enemy;

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

	/// <summary>
	/// マップチップフィールドのSetter
	/// </summary>
	/// <param name="mapChipField"></param>
	void SetMapChipField(const MapChipField* mapChipField) { mapChipField_ = mapChipField; }

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
	/// <param name="enemy">敵</param>
	void OnCollision(const Enemy* enemy);


private:

	/// <summary>
	/// 移動入力
	/// </summary>
	void InputMove();


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 平行光源
	const DirectionalLight* directionalLight_ = nullptr;

	// マップチップフィールド
	const MapChipField* mapChipField_ = nullptr;



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


	// マップとの当たり判定情報
	struct CollisionMapInfo
	{
		// 天井衝突フラグ
		bool isCeilingCollision = false;

		// 着地フラグ
		bool isLanding = false;

		// 壁接触フラグ
		bool isWallCollision = false;

		// 当たり判定に伴う移動量
		Vector3 velocity = { 0.0f , 0.0f , 0.0f };
	};

	// 角
	enum Corner
	{
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		// 要素数
		kNumCorner
	};

	// 当たり判定のサイズ
	const float kCollisionSize = 1.8f;

	/// <summary>
	/// マップとの当たり判定
	/// </summary>
	/// <param name="info">マップとの当たり判定情報</param>
	void MapCollision(CollisionMapInfo& info);

	/// <summary>
	/// マップとの上方向当たり判定
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionTop(CollisionMapInfo& info);

	/// <summary>
	/// マップとの下方向当たり判定
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionBottom(CollisionMapInfo& info);

	/// <summary>
	/// マップとの右方向当たり判定
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionRight(CollisionMapInfo& info);

	/// <summary>
	/// マップとの左方向当たり判定
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionLeft(CollisionMapInfo& info);

	/// <summary>
	/// 角の座標を取得する
	/// </summary>
	/// <param name="center"></param>
	/// <param name="corner"></param>
	/// <returns></returns>
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	/// <summary>
	/// 判定結果を反映して移動させる
	/// </summary>
	/// <param name="info"></param>
	void CollisionMove(const CollisionMapInfo& info);

	/// <summary>
	/// 天井に当たったときの判定処理
	/// </summary>
	/// <param name="info"></param>
	void CeilingCollision(const CollisionMapInfo& info);

	/// <summary>
	/// 壁に当たったときの判定処理
	/// </summary>
	/// <param name="info"></param>
	void WallCollision(const CollisionMapInfo& info);

	/// <summary>
	/// 接地状態の切り替え処理
	/// </summary>
	/// <param name="info"></param>
	void GroundOnSwitch(const CollisionMapInfo& info);


	// 加算するための加速度
	const float kAddAcceleration = 0.03f;

	// 速度減衰率
	const float kAttenuation = 0.3f;

	// 着地時の速度減衰率
	const float kAttenuationLanding = 0.1f;

	// 地面に吸着しないようにするため
	const float kAdsorptionMargin = 0.1f;

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
