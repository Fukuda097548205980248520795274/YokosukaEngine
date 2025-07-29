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

	// 浮遊ギミック初期化
	enemy_->gimmickFloating_->Initialize(bodyWorldTransform, 0.075f);


	// 羽ばたきギミックの生成と初期化
	gimmickFlappingWingR_ = std::make_unique<GimmickFlapping>();
	gimmickFlappingWingR_->Initialize(enemy_->GetWingRWorldTransform(), 0.085f);
	gimmickFlappingWingR_->SetAmplitude(0.75f);

	gimmickFlappingWingL_ = std::make_unique<GimmickFlapping>();
	gimmickFlappingWingL_->Initialize(enemy_->GetWingLWorldTransform(), -0.085f);
	gimmickFlappingWingL_->SetAmplitude(0.75f);
	

	enemy_->GimmickDamageInitialize();
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

	// 羽ばたきギミックの更新
	gimmickFlappingWingR_->Update();
	gimmickFlappingWingL_->Update();

	// ダメージギミック 更新
	enemy_->GimmickDamageUpdate();
}