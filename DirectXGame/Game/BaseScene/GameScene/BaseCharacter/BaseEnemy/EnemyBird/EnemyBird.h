#pragma once
#include "../BaseEnemy.h"

#include "../../../BaseGimmick/GimmickFloating/GimmickFloating.h"

#include "BaseEnemyBirdState/EnemyBirdStateApproachingRear/EnemyBirdStateApproacingRear.h"
#include "BaseEnemyBirdState/EnemyBirdStateAwayTop/EnemyBirdStateAwayTop.h"

class EnemyBird : public BaseEnemy
{
public:

	// モデル列挙体
	enum ModelEnum
	{
		kBody,
		kTopWingR,
		kTopWingL,
		kBottomWingR,
		kBottomWingL,
		kNumModel
	};

	// ステート
	enum State
	{
		kApproachingRear,
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
	/// 右上の羽のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetTopWingRWorldTransform() const { return models_[kTopWingR].worldTransform_.get(); }

	/// <summary>
	/// 左上の羽のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetTopWingLWorldTransform() const { return models_[kTopWingL].worldTransform_.get(); }

	/// <summary>
	/// 右下の羽のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetBottomWingRWorldTransform()  const { return models_[kBottomWingR].worldTransform_.get(); }

	/// <summary>
	/// 左下の羽のワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetBottomWingLWorldTransform()  const { return models_[kBottomWingL].worldTransform_.get(); }

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
	/// 弾を発射する
	/// </summary>
	void BulletShot();

	/// <summary>
	/// ステートを変更
	/// </summary>
	/// <param name="state"></param>
	void ChangeState(State state);


private:

	/// <summary>
	/// ダメージカラー
	/// </summary>
	void DamageColor() override;


	// 状態
	std::unique_ptr<BaseEnemyBirdState> state_ = nullptr;

	// モデル構造体
	ModelStruct models_[kNumModel];

	// モデルの初期位置
	const Vector3 kStartPosition[kNumModel] =
	{
		{0.0f,0.0f,0.0f},
		{0.2f,0.2f,0.0f},
		{-0.2f,0.2f,0.0f},
		{1.2f,0.0f,0.2f},
		{-1.2f,0.0f,0.2f}
	};

	// モデルの初期回転
	const Vector3 kStartRotation[kNumModel] =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;


	// 浮遊ギミック
	std::unique_ptr<GimmickFloating> gimmickFloating_ = nullptr;
};