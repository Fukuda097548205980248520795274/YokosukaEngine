#pragma once
#include "../BaseEnemy.h"

#include "../../../BaseBullet/BaseEnemyBullet/BaseEnemyBullet.h"
#include "../../../BaseBullet/BaseEnemyBullet/EnemyBulletWeek/EnemyBulletWeek.h"

#include "../../../BaseGimmick/GimmickFloating/GimmickFloating.h"

#include "BaseEnemyButterflyState/EnemyButterflyStateApproachingRearLeft/EnemyButterflyStateApproachingRearLeft.h"
#include "BaseEnemyButterflyState/EnemyButterflyStateStop/EnemyButterflyStateStop.h"
#include "BaseEnemyButterflyState/EnemyButterflyStateAwayTop/EnemyButterflyStateAwayTop.h"

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

	// ステート
	enum State
	{
		kApproachingRearLeft,
		kStop,
		kAwayTop
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
	void ChangeState(State state);

	/// <summary>
	/// 弾を発射する
	/// </summary>
	void BulletShot();


private:

	/// <summary>
	/// ダメージカラー
	/// </summary>
	void DamageColor() override;


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
};

