#pragma once
#include "../BaseEnemyDevilState.h"

class EnemyDevilStateStop : public BaseEnemyDevilState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDevilStateStop(EnemyDevil* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

