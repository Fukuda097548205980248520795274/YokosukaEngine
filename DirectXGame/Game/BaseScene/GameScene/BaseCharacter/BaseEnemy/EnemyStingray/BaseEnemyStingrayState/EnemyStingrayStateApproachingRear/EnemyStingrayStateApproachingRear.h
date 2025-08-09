#pragma once
#include <optional>
#include "../BaseEnemyStingrayState.h"
#include "../../BaseEnemyStingrayBehavior/BaseEnemyStingrayBehavior.h"

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


private:

	// ビヘイビア
	enum Behavior
	{
		kNormal,
		kShot
	};

	// ビヘイビア
	std::unique_ptr<BaseEnemyStingrayBehavior> behavior_ = nullptr;

	// 現在のビヘイビア
	Behavior currentBehavior_ = kNormal;

	// 次のビヘイビアのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};

