#include "EnemyButterflyStateStop.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateStop::EnemyButterflyStateStop(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy) {

}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyStateStop::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
}