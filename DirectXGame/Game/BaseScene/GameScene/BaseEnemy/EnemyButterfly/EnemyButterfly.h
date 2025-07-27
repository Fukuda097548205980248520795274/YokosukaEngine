#pragma once
#include "../BaseEnemy.h"

#include "../../BaseEnemyBullet/BaseEnemyBullet.h"
#include "../../BaseEnemyBullet/EnemyBulletWeek/EnemyBulletWeek.h"

class EnemyButterfly : public BaseEnemy
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, const CenterAxis* centerAxis, const Player* target, GameScene* gameScene) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 本体のワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetBodyWorldPosition() const override;

	/// <summary>
	/// 当たり判定用のAABBのGetter
	/// </summary>
	/// <returns></returns>
	AABB GetCollisionAABB() const override;

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="playerBullet"></param>
	void OnCollision(const BasePlayerBullet* playerBullet) override;

	/// <summary>
	/// モデルハンドルのSetter
	/// </summary>
	/// <param name="modelHandles"></param>
	void SetModelHandles(std::vector<uint32_t> modelHandles) override;


private:


	// モデル列挙体
	enum ModelEnum
	{
		kBody,
		kWingR,
		kWingL,
		kNumModel
	};

	// モデル構造体
	ModelStruct models_[kNumModel];

	// モデルの初期位置
	const Vector3 kStartPosition[kNumModel] =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.25f,0.0f},
		{0.0f,0.25f,0.0f}
	};

	// モデルの初期回転
	const Vector3 kStartRotation[kNumModel] =
	{
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f},
		{0.0f , 0.0f , 0.0f}
	};

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;



	/*--------------------
	    ギミック : 浮遊
	--------------------*/

	/// <summary>
	/// ギミック : 浮遊 : 初期化
	/// </summary>
	void GimmickFloatingInitialize();

	/// <summary>
	/// ギミック : 浮遊 : 更新処理
	/// </summary>
	void GimmickFloatingUpdate();

	// 浮遊ギミックのパラメータ
	float floatingParameter_ = 0.0f;

	// 浮遊ギミックの最大値
	const float kFloatingParameterMax = std::numbers::pi_v<float> *2.0f;

	// 浮遊ギミックの速度
	float floatingVelocity_ = 0.0f;

	// 浮遊ギミックの振幅
	float floatingAmplitude_ = 0.0f;


	/*-----------------------
	    ギミック : 羽ばたく
	-----------------------*/

	/// <summary>
	/// ギミック : 羽ばたく : 初期化
	/// </summary>
	void GimmickFlappingInitialize();

	/// <summary>
	/// ギミック : 羽ばたく : 更新処理
	/// </summary>
	void GimmickFlappingUpdate();

	// 羽ばたきギミックのパラメータ
	float flappingParameter_ = 0.0f;

	// 羽ばたきギミックの最大値
	const float kFlappingPrameterMax = std::numbers::pi_v<float> *2.0f;

	// 羽ばたきギミックの速度
	float flappingVelocity_ = 0.0f;

	// 幅だきぎいっくの振幅
	float flappingAmplitude_ = 0.0f;



	/*----------------------
		ギミック : ダメージ
	----------------------*/

	/// <summary>
	/// ギミック : ダメージ : 初期化
	/// </summary>
	void GimmickDamageInitialize();

	/// <summary>
	/// ギミック : ダメージ : 更新処理
	/// </summary>
	void GimmickDamageUpdate();

	/// <summary>
	/// ギミック : ダメージ : 描画処理
	/// </summary>
	void GimmickDamageDraw();

	// ダメージギミックのパラメータ
	float damageParameter_ = 0.1f;

	// ダメージギミックの最大値
	const float kDamageParameterMax = 0.1f;

	// ダメージギミックの速度
	float damageVelocity_ = 1.0f / 60.0f;

	// ダメージの色
	Vector4 damageColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);


	/*--------------------------
	    ギミック : 発射動作
	--------------------------*/
	
	/// <summary>
	/// ギミック : 発射動作 : 初期化
	/// </summary>
	void GimmickShotActionInitliaze();

	/// <summary>
	/// ギミック : 発射動作 : 更新処理
	/// </summary>
	void GimmickShotActionUpdate();

	// 発射動作パラメータ
	float shotActionParameter_ = 0.0f;

	// 発射動作の初期化時の角度
	float shotActionCurrentRotation_ = 0.0f;


	// 発射操作初期回転のパラメータ
	const float kShotActionStartRotationParameter[2] = {0.0f , 0.5f};

	// 発射動作初期回転
	const float kShotActionStartRotation[kNumModel] = { 0.0f ,-2.0f,2.0f };


	// 発射操作回転のパラメータ
	const float kShotActionRotationParameter[2] = { 0.75f , 1.00f };

	// 発射動作回転
	const float kShotActionRotation[kNumModel] = { 0.0f,1.0f,-1.0f };


	// 発射操作終了回転のパラメータ
	const float kShotActionEndRotationParameter[2] = { 2.50f , 3.00f };

	// 発射動作終了回転
	const float kShotActionEndRotation[kNumModel] = { 0.0f,0.0f,0.0f };

	
	// 発射したかどうか
	bool isShot_ = false;



	/*---------------
	    ビヘイビア
	---------------*/

	// ビヘイビア
	enum Behavior
	{
		// 通常
		kNormal,

		// 発射
		kShot,

		// 何もない
		kNothing
	};

	// 現在のビヘイビア
	Behavior behavior_ = kNothing;

	// 次のビヘイビアの予定
	Behavior requestBehavior_ = kNormal;


	/*----------------------
	    ビヘイビア : 通常
	----------------------*/

	/// <summary>
	/// ビヘイビア : 通常 : 初期化
	/// </summary>
	void BehaviorNormalInitialize();

	/// <summary>
	/// ビヘイビア : 通常 : 更新処理
	/// </summary>
	void BehaviorNormalUpdate();

	// 発射までの時間
	const float kShotTime = 3.0f;

	// 発射タイマー
	float shotTimer_ = 0.0f;

	// 発射タイマーの速度
	const float kShotTimerVelocity = 1.0f / 60.0f;

	
	/*----------------------
	    ビヘイビア : 発射
	----------------------*/

	/// <summary>
	/// ビヘイビア : 発射 : 初期化
	/// </summary>
	void BehaviorShotInitialize();

	/// <summary>
	/// ビヘイビア : 発射 : 更新処理
	/// </summary>
	void BehaviorShotUpdate();

	/// <summary>
	/// 弾を発射する
	/// </summary>
	void BulletShot();

	// 発射パラメータ
	float shotParameter_ = 0.0f;

	// 発射パラメータの最大値
	const float kShotParameterMax = 3.0f;

	// 発射パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;
};

