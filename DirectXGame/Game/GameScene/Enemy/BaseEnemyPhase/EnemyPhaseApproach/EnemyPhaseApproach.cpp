#include "EnemyPhaseApproach.h"
#include "../../Enemy.h"
#include "../../BaseEnemyPhase/EnemyPhaseLeave/EnemyPhaseLeave.h"

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyPhaseApproach::EnemyPhaseApproach(Enemy* enemy)
	: BaseEnemyPhase("State Approach", enemy) {
};

/// <summary>
/// 更新処理
/// </summary>
void EnemyPhaseApproach::Update()
{
	// 移動速度
	const float kMoveSpeed = 0.1f;

	// 座標移動する
	enemy_->SetWorldTransformTranslation(enemy_->GetWorldTransformTranslation() + Vector3(0.0f, 0.0f, -kMoveSpeed));



	// 発射するタイマーを進める
	enemy_->SetShotTimer(enemy_->GetShotTimer() + (1.0f / 60.0f));

	// 発射間隔を越えたら、発射する
	if (enemy_->GetShotTimer() >= enemy_->GetKShotInterval())
	{
		enemy_->BulletShot();
	}



	// 離脱フェーズに切り替える
	if (enemy_->GetWorldTransformTranslation().z <= 0.0f)
	{
		enemy_->ChangePhase(std::make_unique<EnemyPhaseLeave>(enemy_));
	}
}