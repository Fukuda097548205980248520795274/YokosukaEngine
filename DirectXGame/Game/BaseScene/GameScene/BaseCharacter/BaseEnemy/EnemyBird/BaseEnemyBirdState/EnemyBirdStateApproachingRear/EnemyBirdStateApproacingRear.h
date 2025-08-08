#pragma once
#include "../BaseEnemyBirdState.h"

class EnemyBirdStateApproacingRear : public BaseEnemyBirdState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyBirdStateApproacingRear(EnemyBird* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

