#include "EnemyButterflyStateAwayTop.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateAwayTop::EnemyButterflyStateAwayTop(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy)
{
	behavior_ = std::make_unique<EnemyButterflyBehaviorNormal>(enemy);

	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyStateAwayTop::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
	worldTransform->translation_.y += 1.0f;

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// ビヘイビア更新
	behavior_->Update();

	timer_ += 1.0f / 60.0f;

	// タイマーを越えたら消滅する
	if (timer_ >= time_)
	{
		enemy_->FinisheIt();
	}
}