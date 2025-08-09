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


	// 羽ばたきギミックの生成と初期化
	gimmickFlappingWingR_ = std::make_unique<GimmickFlapping>();
	gimmickFlappingWingR_->SetGameTimer(enemy_->GetGameTimer());
	gimmickFlappingWingR_->Initialize(enemy_->GetWingRWorldTransform(), 0.085f);
	gimmickFlappingWingR_->SetAmplitude(0.75f);
	gimmickFlappingWingR_->SetRotationAxis(GimmickFlapping::kY);
	gimmickFlappingWingR_->SetStartRotation(0.0f);

	gimmickFlappingWingL_ = std::make_unique<GimmickFlapping>();
	gimmickFlappingWingL_->SetGameTimer(enemy_->GetGameTimer());
	gimmickFlappingWingL_->Initialize(enemy_->GetWingLWorldTransform(), -0.085f);
	gimmickFlappingWingL_->SetAmplitude(0.75f);
	gimmickFlappingWingL_->SetRotationAxis(GimmickFlapping::kY);
	gimmickFlappingWingL_->SetStartRotation(0.0f);
	
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyBehaviorNormal::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新
	gimmickFlappingWingR_->Update();
	gimmickFlappingWingL_->Update();


	// タイマーを進める
	shotTimer_ += kShotTimerVelocity * (*gameTimer);
	shotTimer_ = std::min(shotTimer_, kShotTime);

	// 時間が終わったら終了する
	if (shotTimer_ >= kShotTime)
	{
		isFinished_ = true;
	}
}