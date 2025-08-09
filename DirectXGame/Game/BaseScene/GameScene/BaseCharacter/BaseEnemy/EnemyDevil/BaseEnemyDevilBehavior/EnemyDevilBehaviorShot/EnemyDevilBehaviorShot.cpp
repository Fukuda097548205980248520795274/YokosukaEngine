#include "EnemyDevilBehaviorShot.h"
#include "../../EnemyDevil.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyDevilBehaviorShot::EnemyDevilBehaviorShot(EnemyDevil* enemy) : BaseEnemyDevilBehavior(enemy)
{

}

/// <summary>
/// 更新処理
/// </summary>
void EnemyDevilBehaviorShot::Update()
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