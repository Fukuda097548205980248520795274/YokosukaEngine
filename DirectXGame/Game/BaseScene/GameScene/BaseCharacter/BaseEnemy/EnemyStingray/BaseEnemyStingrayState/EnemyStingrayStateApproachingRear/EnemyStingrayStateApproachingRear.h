#pragma once
#include "../BaseEnemyStingrayState.h"

class EnemyStingrayStateApproachingRear : public BaseEnemyStingrayState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyStingrayStateApproachingRear(EnemyStingray* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

