#pragma once
#include "../BaseEnemyBirdBehavior.h"

class EnemyBirdBehaviorNormal : public BaseEnemyBirdBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyBirdBehaviorNormal(EnemyBird* enemy);

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
};

