#pragma once
#include "../BaseEnemy.h"

#include "../../../BaseBullet/BaseEnemyBullet/BaseEnemyBullet.h"
#include "../../../BaseBullet/BaseEnemyBullet/EnemyBulletWeek/EnemyBulletWeek.h"

#include "../../../BaseGimmick/GimmickFloating/GimmickFloating.h"

#include "BaseEnemyButterflyState/EnemyButterflyStateApproachingRearLeft/EnemyButterflyStateApproachingRearLeft.h"
#include "BaseEnemyButterflyState/EnemyButterflyStateStop/EnemyButterflyStateStop.h"

#include "BaseEnemyButterflyBehavior/EnemyButterflyBehaviorNormal/EnemyButterflyBehaviorNormal.h"
#include "BaseEnemyButterflyBehavior/EnemyButterflyBehaviorShot/EnemyButterflyBehaviorShot.h"

class EnemyButterfly : public BaseEnemy
{
public:

	// モデル列挙体
	enum ModelEnum
	{
		kBody,
		kWingR,
		kWingL,
		kNumModel
	};


public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const Vector3& position, int32_t hp) override;

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
	/// 本体のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetBodyWorldTransform() const override { return models_[kBody].worldTransform_.get(); }

	/// <summary>
	/// 右の羽のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWingRWorldTransform() const { return models_[kWingR].worldTransform_.get(); }

	/// <summary>
	/// 左の羽のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWingLWorldTransform() const { return models_[kWingL].worldTransform_.get(); }

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
	/// ステートを変更
	/// </summary>
	/// <param name="state"></param>
	void ChangeState(std::unique_ptr<BaseEnemyButterflyState> state);

	/// <summary>
	/// 弾を発射する
	/// </summary>
	void BulletShot();


	// 浮遊ギミック
	std::unique_ptr<GimmickFloating> gimmickFloating_ = nullptr;








	/// <summary>
	/// ギミック : 羽ばたく : 初期化
	/// </summary>
	void GimmickFlappingInitialize();

	/// <summary>
	/// ギミック : 羽ばたく : 更新処理
	/// </summary>
	void GimmickFlappingUpdate();





	/// <summary>
	/// ギミック : ダメージ : 初期化
	/// </summary>
	void GimmickDamageInitialize();

	/// <summary>
	/// ギミック : ダメージ : 更新処理
	/// </summary>
	void GimmickDamageUpdate();

private:


	// 状態
	std::unique_ptr<BaseEnemyButterflyState> state_ = nullptr;

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


	/*-----------------------
	    ギミック : 羽ばたく
	-----------------------*/

	

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
};

