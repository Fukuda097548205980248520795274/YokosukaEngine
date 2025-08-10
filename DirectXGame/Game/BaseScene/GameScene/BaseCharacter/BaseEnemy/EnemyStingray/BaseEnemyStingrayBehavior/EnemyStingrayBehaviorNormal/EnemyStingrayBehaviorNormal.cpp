#include "EnemyStingrayBehaviorNormal.h"
#include "../../EnemyStingray.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyStingrayBehaviorNormal::EnemyStingrayBehaviorNormal(EnemyStingray* enemy) : BaseEnemyStingrayBehavior(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();

	// 発射タイマー
	shotTimer_ = 0.0f;



	// 羽ばたきギミックの生成と初期化
	flappingWingR_ = std::make_unique<GimmickFlapping>();
	flappingWingR_->SetGameTimer(enemy_->GetGameTimer());
	flappingWingR_->Initialize(enemy_->GetWingRWorldTransform(), 0.085f);
	flappingWingR_->SetAmplitude(0.2f);
	flappingWingR_->SetRotationAxis(GimmickFlapping::kZ);
	flappingWingR_->SetStartRotation(0.0f);

	flappingWingL_ = std::make_unique<GimmickFlapping>();
	flappingWingL_->SetGameTimer(enemy_->GetGameTimer());
	flappingWingL_->Initialize(enemy_->GetWingLWorldTransform(), -0.085f);
	flappingWingL_->SetAmplitude(0.2f);
	flappingWingL_->SetRotationAxis(GimmickFlapping::kZ);
	flappingWingL_->SetStartRotation(0.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyStingrayBehaviorNormal::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新
	flappingWingR_->Update();
	flappingWingL_->Update();


	// タイマーを進める
	shotTimer_ += kShotTimerVelocity * (*gameTimer);
	shotTimer_ = std::min(shotTimer_, kShotTime);

	// 時間が終わったら終了する
	if (shotTimer_ >= kShotTime)
	{
		isFinished_ = true;
	}
}