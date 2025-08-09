#pragma once
#include <optional>
#include "../BaseEnemyBirdState.h"
#include "../../BaseEnemyBirdBehavior/BaseEnemyBirdBehavior.h"

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


private:

	// ビヘイビア
	enum Behavior
	{
		kNormal,
		kShot
	};

	// ビヘイビア
	std::unique_ptr<BaseEnemyBirdBehavior> behavior_ = nullptr;

	// 現在のビヘイビア
	Behavior currentBehavior_ = kNormal;

	// 次のビヘイビアのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};

