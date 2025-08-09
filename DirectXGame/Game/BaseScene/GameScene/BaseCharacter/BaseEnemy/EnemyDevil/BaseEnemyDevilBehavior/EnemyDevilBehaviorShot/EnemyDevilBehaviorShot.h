#pragma once
#include "../BaseEnemyDevilBehavior.h"

class EnemyDevilBehaviorShot : public BaseEnemyDevilBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDevilBehaviorShot(EnemyDevil* enemy);

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

