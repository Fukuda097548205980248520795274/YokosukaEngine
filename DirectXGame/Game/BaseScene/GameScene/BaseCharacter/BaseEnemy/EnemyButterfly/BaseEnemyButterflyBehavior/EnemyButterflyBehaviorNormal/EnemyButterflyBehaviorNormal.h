#pragma once
#include "../BaseEnemyButterflyBehavior.h"
#include "../../../../../BaseGimmick/GimmickFlapping/GimmickFlapping.h"

class EnemyButterflyBehaviorNormal : public BaseEnemyButterflyBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyButterflyBehaviorNormal(EnemyButterfly* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;


private:

	// 発射までの時間
	const float kShotTime = 1.0f;

	// 発射タイマー
	float shotTimer_ = 0.0f;

	// 発射タイマーの速度
	const float kShotTimerVelocity = 1.0f / 60.0f;


	// 羽ばたきギミック
	std::unique_ptr<GimmickFlapping> gimmickFlappingWingR_ = nullptr;
	std::unique_ptr<GimmickFlapping> gimmickFlappingWingL_ = nullptr;
};

