#pragma once
#include "../BaseEnemyDevilState.h"

#include "../../../../../BaseGimmick/GimmickFlapping/GimmickFlapping.h"

class EnemyDevilStateAwayTop : public BaseEnemyDevilState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDevilStateAwayTop(EnemyDevil* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


private:

	// 羽ばたき
	std::unique_ptr<GimmickFlapping> flappingWingR_ = nullptr;
	std::unique_ptr<GimmickFlapping> flappingWingL_ = nullptr;
};

