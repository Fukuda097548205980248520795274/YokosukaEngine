#pragma once
#include "../BaseEnemyBirdBehavior.h"

class EnemyBirdBehaviorShot : public BaseEnemyBirdBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyBirdBehaviorShot(EnemyBird* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;


private:


	// 発射パラメータ
	float shotParameter_ = 0.0f;

	// 発射パラメータの最大値
	const float kShotParameterMax = 3.0f;

	// 発射パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;
};

