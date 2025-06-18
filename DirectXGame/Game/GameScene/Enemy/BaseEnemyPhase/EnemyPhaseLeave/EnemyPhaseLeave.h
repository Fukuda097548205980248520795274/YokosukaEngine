#pragma once
#include "../BaseEnemyPhase.h"

class EnemyPhaseLeave : public BaseEnemyPhase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyPhaseLeave(Enemy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
};

