#include "EnemyButterflyBehaviorNormal.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyBehaviorNormal::EnemyButterflyBehaviorNormal(EnemyButterfly* enemy) : BaseEnemyButterflyBehavior(enemy) 
{
	// ワールドトランスフォームを取得する
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();

	// 発射タイマー
	shotTimer_ = 0.0f;

	// 羽ばたきギミック初期化
	enemy_->gimmickFloating_->Initialize(bodyWorldTransform, 0.075f);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyBehaviorNormal::Update()
{
	// タイマーを進める
	shotTimer_ += kShotTimerVelocity;
	shotTimer_ = std::min(shotTimer_, kShotTime);

	// 発射ビヘイビアに遷移する
	if (shotTimer_ >= kShotTime)
	{
		return;
	}


	// 浮遊ギミック 更新
	enemy_->gimmickFloating_->Update();

	// 羽ばたきギミック 更新
	enemy_->GimmickFlappingUpdate();

	// ダメージギミック 更新
	enemy_->GimmickDamageUpdate();
}