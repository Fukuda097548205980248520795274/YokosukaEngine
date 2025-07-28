#pragma once
#include "../BaseEnemyButterflyState.h"

class EnemyButterflyStateStop : public BaseEnemyButterflyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyButterflyStateStop(EnemyButterfly* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

