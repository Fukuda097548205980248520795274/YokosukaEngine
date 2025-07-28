#pragma once
#include "../BaseEnemyButterflyState.h"

class EnemyButterflyStateApproachingRearLeft : BaseEnemyButterflyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyButterflyStateApproachingRearLeft(EnemyButterfly* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


private:

	// 時間
	float time_ = 4.0f;

	// タイマー
	float timer_ = 0.0f;
};

