#include "EnemyPhaseLeave.h"
#include "../../Enemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyPhaseLeave::EnemyPhaseLeave(Enemy* enemy)
	: BaseEnemyPhase("State Leave", enemy) {
};

/// <summary>
/// 更新処理
/// </summary>
void EnemyPhaseLeave::Update()
{
	// 移動速度
	const float kMoveSpeed = 0.3f;

	// 速度ベクトル
	Vector3 velocity = { 0.0f , 1.0f , 0.0f };

	// 座標移動する
	enemy_->SetWorldTransformTranslation(enemy_->GetWorldTransformTranslation() + (kMoveSpeed * velocity));
}