#pragma once
#include "../BaseEnemyPhase.h"

class EnemyPhaseApproach : public BaseEnemyPhase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">状態名</param>
	/// <param name="enemy">敵のポインタ</param>
	EnemyPhaseApproach(Enemy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
};

