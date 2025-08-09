#pragma once
#include "../BaseEnemyDevilState.h"

class EnemyDevilStateApproachingRear : public BaseEnemyDevilState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDevilStateApproachingRear(EnemyDevil* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

