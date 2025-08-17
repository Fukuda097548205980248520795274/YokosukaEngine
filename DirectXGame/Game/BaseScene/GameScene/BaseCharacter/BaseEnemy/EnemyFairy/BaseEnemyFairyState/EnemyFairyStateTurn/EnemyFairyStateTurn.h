#pragma once
#include "../BaseEnemyFairyState.h"

class EnemyFairyStateTurn : public BaseEnemyFairyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyStateTurn(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

