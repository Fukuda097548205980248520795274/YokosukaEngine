#include "EnemyButterflyStateApproachingRearLeft.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateApproachingRearLeft::EnemyButterflyStateApproachingRearLeft(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy) 
{
	behavior_ = std::make_unique<EnemyButterflyBehaviorShot>(enemy);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyStateApproachingRearLeft::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ビヘイビア更新
	behavior_->Update();

	// タイマーを越えたら停止状態に移行する
	if (timer_ >= time_)
	{
		
	}
}