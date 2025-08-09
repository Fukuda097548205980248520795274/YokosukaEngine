#include "EnemyBirdBehaviorNormal.h"
#include "../../EnemyBird.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyBirdBehaviorNormal::EnemyBirdBehaviorNormal(EnemyBird* enemy) : BaseEnemyBirdBehavior(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();

	// 発射タイマー
	shotTimer_ = 0.0f;

}

/// <summary>
/// 更新処理
/// </summary>
void EnemyBirdBehaviorNormal::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// タイマーを進める
	shotTimer_ += kShotTimerVelocity * (*gameTimer);
	shotTimer_ = std::min(shotTimer_, kShotTime);

	// 時間が終わったら終了する
	if (shotTimer_ >= kShotTime)
	{
		isFinished_ = true;
	}
}