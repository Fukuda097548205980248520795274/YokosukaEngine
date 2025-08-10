#pragma once
#include "../BaseEnemyFairyState.h"
#include "../../BaseEnemyFairyBehavior/BaseEnemyFairyBehavior.h"

#include "../../../../../BaseGimmick/GimmickFlapping/GimmickFlapping.h"

class EnemyFairyStateStop : public BaseEnemyFairyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyStateStop(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


private:

	// ビヘイビア
	enum Behavior
	{
		kNormal,
		kShot
	};

	// ビヘイビア
	std::unique_ptr<BaseEnemyFairyBehavior> behavior_ = nullptr;

	// 現在のビヘイビア
	Behavior currentBehavior_ = kNormal;

	// 次のビヘイビアのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;


	// 羽ばたき
	std::unique_ptr<GimmickFlapping> flappingWingR_ = nullptr;
	std::unique_ptr<GimmickFlapping> flappingWingL_ = nullptr;
};

