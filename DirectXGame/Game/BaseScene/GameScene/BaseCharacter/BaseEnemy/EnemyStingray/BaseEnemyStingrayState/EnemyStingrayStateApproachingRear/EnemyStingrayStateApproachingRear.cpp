#include "EnemyStingrayStateApproachingRear.h"
#include "../../EnemyStingray.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyStingrayStateApproachingRear::EnemyStingrayStateApproachingRear(EnemyStingray* enemy)
	: BaseEnemyStingrayState(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
	worldTransform->translation_.z += -50.0f;


	// 時間
	time_ = 2.0f;

	// タイマー
	timer_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyStingrayStateApproachingRear::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 奥に進む
	worldTransform->translation_.z += 1.0f * (*gameTimer);

	// タイマーを進める
	timer_ += (1.0f / 60.0f) * (*gameTimer);

	// タイマーを越えたら停止状態に移行する
	if (timer_ >= time_)
	{
		enemy_->ChangeState(EnemyStingray::kAwayTop);
	}
}