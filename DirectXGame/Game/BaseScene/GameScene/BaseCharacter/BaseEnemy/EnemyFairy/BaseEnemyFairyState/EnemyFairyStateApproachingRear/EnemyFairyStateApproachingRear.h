#pragma once
#include "../BaseEnemyFairyState.h"

#include "../../../../../BaseGimmick/GimmickFlapping/GimmickFlapping.h"

class EnemyFairyStateApproachingRear : public BaseEnemyFairyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyStateApproachingRear(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


private:

	// 腕の羽ばたき
	std::unique_ptr<GimmickFlapping> flappingArmR_ = nullptr;
	std::unique_ptr<GimmickFlapping> flappingArmL_ = nullptr;
};

