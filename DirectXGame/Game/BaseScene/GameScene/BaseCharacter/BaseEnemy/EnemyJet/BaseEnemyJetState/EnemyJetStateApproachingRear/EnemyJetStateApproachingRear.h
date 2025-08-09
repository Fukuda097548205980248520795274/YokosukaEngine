#pragma once
#include "../BaseEnemyJetState.h"
#include "../../BaseEnemyJetBehavior/BaseEnemyJetBehavior.h"

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


private:

	// ビヘイビア
	enum Behavior
	{
		kNormal,
		kShot
	};

	// ビヘイビア
	std::unique_ptr<BaseEnemyJetBehavior> behavior_ = nullptr;

	// 現在のビヘイビア
	Behavior currentBehavior_ = kNormal;

	// 次のビヘイビアのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};

