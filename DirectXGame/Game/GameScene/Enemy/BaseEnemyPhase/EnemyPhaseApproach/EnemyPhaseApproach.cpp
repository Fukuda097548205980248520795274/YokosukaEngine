#include "EnemyPhaseApproach.h"
#include "../../Enemy.h"
#include "../../BaseEnemyPhase/EnemyPhaseLeave/EnemyPhaseLeave.h"

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyPhaseApproach::EnemyPhaseApproach(Enemy* enemy)
	: BaseEnemyPhase("State Approach", enemy) 
{
	// 弾発射前のタイマーリセット
	enemy_->BulletShotPrevReset();
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



	// 離脱フェーズに切り替える
	if (enemy_->GetWorldTransformTranslation().z <= 0.0f)
	{
		// 弾発射の時限発動を消す
		enemy_->BulletShotTimerDelete();

		enemy_->ChangePhase(std::make_unique<EnemyPhaseLeave>(enemy_));
	}
}