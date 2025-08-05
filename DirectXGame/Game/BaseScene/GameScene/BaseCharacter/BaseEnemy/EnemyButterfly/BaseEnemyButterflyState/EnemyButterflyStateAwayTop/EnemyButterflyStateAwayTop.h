#pragma once
#include "../BaseEnemyButterflyState.h"
#include "../../BaseEnemyButterflyBehavior/BaseEnemyButterflyBehavior.h"

#include "../../../../../BaseGimmick/GimmickFlapping/GimmickFlapping.h"

class EnemyButterflyStateAwayTop : public BaseEnemyButterflyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyButterflyStateAwayTop(EnemyButterfly* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


private:

	// 羽ばたきギミック
	std::unique_ptr<GimmickFlapping> gimmickFlappingWingR_ = nullptr;
	std::unique_ptr<GimmickFlapping> gimmickFlappingWingL_ = nullptr;
};

