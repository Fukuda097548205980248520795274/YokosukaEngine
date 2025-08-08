#pragma once
#include "../BaseEnemyFairyState.h"

class EnemyFairyStateApproachingRear : public BaseEnemyFairyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyStateApproachingRear(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

