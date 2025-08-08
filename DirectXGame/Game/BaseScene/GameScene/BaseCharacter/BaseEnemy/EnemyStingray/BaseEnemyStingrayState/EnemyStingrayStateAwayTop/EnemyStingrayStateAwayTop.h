#pragma once
#include "../BaseEnemyStingrayState.h"

class EnemyStingrayStateAwayTop : public BaseEnemyStingrayState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyStingrayStateAwayTop(EnemyStingray* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

