#pragma once
#include "../BaseEnemyDevilState.h"
#include "../../BaseEnemyDevilBehavior/BaseEnemyDevilBehavior.h"

class EnemyDevilStateStop : public BaseEnemyDevilState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDevilStateStop(EnemyDevil* enemy);

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
	std::unique_ptr<BaseEnemyDevilBehavior> behavior_ = nullptr;

	// 現在のビヘイビア
	Behavior currentBehavior_ = kNormal;

	// 次のビヘイビアのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};

