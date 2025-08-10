#include "EnemyFairyBehaviorNormal.h"
#include "../../EnemyFairy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyFairyBehaviorNormal::EnemyFairyBehaviorNormal(EnemyFairy* enemy) : BaseEnemyFairyBehavior(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();

	// 発射タイマー
	shotTimer_ = 0.0f;


	// 羽ばたきギミックの生成と初期化
	flappingArmR_ = std::make_unique<GimmickFlapping>();
	flappingArmR_->SetGameTimer(enemy_->GetGameTimer());
	flappingArmR_->Initialize(enemy_->GetArmRWorldTransform(), 0.085f);
	flappingArmR_->SetAmplitude(0.2f);
	flappingArmR_->SetRotationAxis(GimmickFlapping::kZ);
	flappingArmR_->SetStartRotation(-0.8f);

	flappingArmL_ = std::make_unique<GimmickFlapping>();
	flappingArmL_->SetGameTimer(enemy_->GetGameTimer());
	flappingArmL_->Initialize(enemy_->GetArmLWorldTransform(), -0.085f);
	flappingArmL_->SetAmplitude(0.2f);
	flappingArmL_->SetRotationAxis(GimmickFlapping::kZ);
	flappingArmL_->SetStartRotation(0.8f);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyFairyBehaviorNormal::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新処理
	flappingArmR_->Update();
	flappingArmL_->Update();

	// タイマーを進める
	shotTimer_ += kShotTimerVelocity * (*gameTimer);
	shotTimer_ = std::min(shotTimer_, kShotTime);

	// 時間が終わったら終了する
	if (shotTimer_ >= kShotTime)
	{
		isFinished_ = true;
	}
}