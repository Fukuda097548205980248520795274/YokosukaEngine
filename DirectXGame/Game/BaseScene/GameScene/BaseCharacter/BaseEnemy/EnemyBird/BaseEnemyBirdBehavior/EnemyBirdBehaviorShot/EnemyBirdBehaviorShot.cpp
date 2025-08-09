#include "EnemyBirdBehaviorShot.h"
#include "../../EnemyBird.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyBirdBehaviorShot::EnemyBirdBehaviorShot(EnemyBird* enemy) : BaseEnemyBirdBehavior(enemy)
{

}

/// <summary>
/// 更新処理
/// </summary>
void EnemyBirdBehaviorShot::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();

	// 発射パラメータを進める
	shotParameter_ += kShotParameterVelocity * (*gameTimer);
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);

	// 発射パラメータが最大値で終了する
	if (shotParameter_ >= kShotParameterMax)
	{
		isFinished_ = true;
		return;
	}
}