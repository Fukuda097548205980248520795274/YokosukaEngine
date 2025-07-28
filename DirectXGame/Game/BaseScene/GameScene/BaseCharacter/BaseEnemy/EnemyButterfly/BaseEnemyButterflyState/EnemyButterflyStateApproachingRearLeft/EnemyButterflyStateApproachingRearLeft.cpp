#include "EnemyButterflyStateApproachingRearLeft.h"
#include "../../EnemyButterfly.h"
#include "../EnemyButterflyStateStop/EnemyButterflyStateStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateApproachingRearLeft::EnemyButterflyStateApproachingRearLeft(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy) {

}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyStateApproachingRearLeft::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// タイマーを越えたら停止状態に移行する
	if (timer_ >= time_)
	{
		enemy_->ChangeState(std::move(std::make_unique<EnemyButterflyStateStop>(enemy_)));
	}
}