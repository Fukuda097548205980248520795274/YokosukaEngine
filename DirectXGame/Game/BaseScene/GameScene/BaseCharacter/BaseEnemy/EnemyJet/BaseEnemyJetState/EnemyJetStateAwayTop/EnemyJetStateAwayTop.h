#pragma once
#include "../BaseEnemyJetState.h"

class EnemyJetStateAwayTop : public BaseEnemyJetState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyJetStateAwayTop(EnemyJet* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
};

