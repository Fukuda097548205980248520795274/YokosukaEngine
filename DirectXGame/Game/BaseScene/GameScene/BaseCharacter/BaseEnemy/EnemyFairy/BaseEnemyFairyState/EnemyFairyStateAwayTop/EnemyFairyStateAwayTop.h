#pragma once
#include "../BaseEnemyFairyState.h"

#include "../../../../../BaseGimmick/GimmickFlapping/GimmickFlapping.h"

class EnemyFairyStateAwayTop : public BaseEnemyFairyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyStateAwayTop(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


private:

	// 羽ばたき
	std::unique_ptr<GimmickFlapping> flappingWingR_ = nullptr;
	std::unique_ptr<GimmickFlapping> flappingWingL_ = nullptr;
};

