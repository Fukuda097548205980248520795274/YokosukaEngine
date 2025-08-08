#pragma once
#include "../BaseEnemyBirdState.h"

class EnemyBirdStateAwayTop : public BaseEnemyBirdState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyBirdStateAwayTop(EnemyBird* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

