#include "EnemyStingrayBehaviorShot.h"
#include "../../EnemyStingray.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyStingrayBehaviorShot::EnemyStingrayBehaviorShot(EnemyStingray* enemy) : BaseEnemyStingrayBehavior(enemy)
{

}

/// <summary>
/// 更新処理
/// </summary>
void EnemyStingrayBehaviorShot::Update()
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