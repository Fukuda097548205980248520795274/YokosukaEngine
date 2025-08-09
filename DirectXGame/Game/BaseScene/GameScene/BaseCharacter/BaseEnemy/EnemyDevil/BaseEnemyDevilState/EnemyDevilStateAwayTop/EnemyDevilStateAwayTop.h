#pragma once
#include "../BaseEnemyDevilState.h"

class EnemyDevilStateAwayTop : public BaseEnemyDevilState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDevilStateAwayTop(EnemyDevil* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

