#pragma once
#include "../BaseEnemyButterflyState.h"

#include "../../BaseEnemyButterflyBehavior/BaseEnemyButterflyBehavior.h"

class EnemyButterflyStateApproachingRearLeft : public BaseEnemyButterflyState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyButterflyStateApproachingRearLeft(EnemyButterfly* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;


private:

	// 時間
	float time_ = 2.0f;

	// タイマー
	float timer_ = 0.0f;

	// ビヘイビア
	std::unique_ptr<BaseEnemyButterflyBehavior> behavior_ = nullptr;
};

