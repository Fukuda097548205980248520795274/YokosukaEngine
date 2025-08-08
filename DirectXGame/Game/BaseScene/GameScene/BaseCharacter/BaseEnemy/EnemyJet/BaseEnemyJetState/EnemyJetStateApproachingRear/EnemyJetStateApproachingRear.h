#pragma once
#include "../BaseEnemyJetState.h"

class EnemyJetStateApproachingRear : public BaseEnemyJetState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyJetStateApproachingRear(EnemyJet* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

