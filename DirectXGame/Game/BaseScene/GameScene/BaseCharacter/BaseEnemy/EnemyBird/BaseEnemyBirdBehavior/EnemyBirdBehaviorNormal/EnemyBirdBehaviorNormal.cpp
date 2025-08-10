#include "EnemyBirdBehaviorNormal.h"
#include "../../EnemyBird.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyBirdBehaviorNormal::EnemyBirdBehaviorNormal(EnemyBird* enemy) : BaseEnemyBirdBehavior(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();

	// 発射タイマー
	shotTimer_ = 0.0f;


	// 羽ばたきギミックの生成と初期化
	flappingTopWingR_ = std::make_unique<GimmickFlapping>();
	flappingTopWingR_->SetGameTimer(enemy_->GetGameTimer());
	flappingTopWingR_->Initialize(enemy_->GetTopWingRWorldTransform(), 0.085f);
	flappingTopWingR_->SetAmplitude(0.2f);
	flappingTopWingR_->SetRotationAxis(GimmickFlapping::kZ);
	flappingTopWingR_->SetStartRotation(0.0f);

	flappingTopWingL_ = std::make_unique<GimmickFlapping>();
	flappingTopWingL_->SetGameTimer(enemy_->GetGameTimer());
	flappingTopWingL_->Initialize(enemy_->GetTopWingLWorldTransform(), -0.085f);
	flappingTopWingL_->SetAmplitude(0.2f);
	flappingTopWingL_->SetRotationAxis(GimmickFlapping::kZ);
	flappingTopWingL_->SetStartRotation(0.0f);

	flappingBottomWingR_ = std::make_unique<GimmickFlapping>();
	flappingBottomWingR_->SetGameTimer(enemy_->GetGameTimer());
	flappingBottomWingR_->Initialize(enemy_->GetBottomWingRWorldTransform(), 0.085f);
	flappingBottomWingR_->SetAmplitude(0.2f);
	flappingBottomWingR_->SetRotationAxis(GimmickFlapping::kZ);
	flappingBottomWingR_->SetStartRotation(0.0f);

	flappingBottomWingL_ = std::make_unique<GimmickFlapping>();
	flappingBottomWingL_->SetGameTimer(enemy_->GetGameTimer());
	flappingBottomWingL_->Initialize(enemy_->GetBottomWingLWorldTransform(), -0.085f);
	flappingBottomWingL_->SetAmplitude(0.2f);
	flappingBottomWingL_->SetRotationAxis(GimmickFlapping::kZ);
	flappingBottomWingL_->SetStartRotation(0.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyBirdBehaviorNormal::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミック更新
	flappingTopWingL_->Update();
	flappingTopWingR_->Update();
	flappingBottomWingL_->Update();
	flappingBottomWingR_->Update();


	// タイマーを進める
	shotTimer_ += kShotTimerVelocity * (*gameTimer);
	shotTimer_ = std::min(shotTimer_, kShotTime);

	// 時間が終わったら終了する
	if (shotTimer_ >= kShotTime)
	{
		isFinished_ = true;
	}
}