#pragma once
#include "../BaseEnemyFairyState.h"

class EnemyFairyStateAwayTop : public BaseEnemyFairyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyStateAwayTop(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

