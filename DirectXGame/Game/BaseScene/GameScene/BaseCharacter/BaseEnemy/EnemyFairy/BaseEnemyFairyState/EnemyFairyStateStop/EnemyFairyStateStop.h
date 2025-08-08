#pragma once
#include "../BaseEnemyFairyState.h"

class EnemyFairyStateStop : public BaseEnemyFairyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyStateStop(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

