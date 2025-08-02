#include "EnemyButterflyStateApproachingRearLeft.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateApproachingRearLeft::EnemyButterflyStateApproachingRearLeft(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy) 
{
	behavior_ = std::make_unique<EnemyButterflyBehaviorNormal>(enemy);

	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
	worldTransform->translation_.z += -50.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyStateApproachingRearLeft::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();

	// 奥に進む
	worldTransform->translation_.z += 1.0f * (*gameTimer);


	// ビヘイビア更新
	behavior_->Update();

	timer_ += (1.0f / 60.0f) * (*gameTimer);

	// タイマーを越えたら停止状態に移行する
	if (timer_ >= time_)
	{
		enemy_->ChangeState(EnemyButterfly::kStop);
	}
}