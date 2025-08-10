#include "EnemyFairyStateApproachingRear.h"
#include "../../EnemyFairy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyFairyStateApproachingRear::EnemyFairyStateApproachingRear(EnemyFairy* enemy)
	: BaseEnemyFairyState(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();


	// 時間
	time_ = 2.0f;

	// タイマー
	timer_ = 0.0f;


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
void EnemyFairyStateApproachingRear::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新
	flappingArmR_->Update();
	flappingArmL_->Update();

	// 奥に進む
	worldTransform->translation_.z += 1.0f * (*gameTimer);

	// タイマーを進める
	timer_ += (1.0f / 60.0f) * (*gameTimer);

	// タイマーを越えたら停止状態に移行する
	if (timer_ >= time_)
	{
		enemy_->ChangeState(EnemyFairy::kStop);
	}
}