#include "EnemyDevilStateApproachingRear.h"
#include "../../EnemyDevil.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyDevilStateApproachingRear::EnemyDevilStateApproachingRear(EnemyDevil* enemy)
	: BaseEnemyDevilState(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
	worldTransform->translation_.z += -50.0f;


	// 時間
	time_ = 2.0f;

	// タイマー
	timer_ = 0.0f;


	// 羽ばたきギミックの生成と初期化
	flappingWingR_ = std::make_unique<GimmickFlapping>();
	flappingWingR_->SetGameTimer(enemy_->GetGameTimer());
	flappingWingR_->Initialize(enemy_->GetWingRWorldTransform(), 0.2f);
	flappingWingR_->SetAmplitude(0.4f);
	flappingWingR_->SetRotationAxis(GimmickFlapping::kY);
	flappingWingR_->SetStartRotation(-0.5f);

	flappingWingL_ = std::make_unique<GimmickFlapping>();
	flappingWingL_->SetGameTimer(enemy_->GetGameTimer());
	flappingWingL_->Initialize(enemy_->GetWingLWorldTransform(), -0.2f);
	flappingWingL_->SetAmplitude(0.4f);
	flappingWingL_->SetRotationAxis(GimmickFlapping::kY);
	flappingWingL_->SetStartRotation(0.5f);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyDevilStateApproachingRear::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新
	flappingWingR_->Update();
	flappingWingL_->Update();


	// 奥に進む
	worldTransform->translation_ += enemy_->GetApproachingDirection() * (*gameTimer);

	// タイマーを進める
	timer_ += (1.0f / 60.0f) * (*gameTimer);

	// タイマーを越えたら停止状態に移行する
	if (timer_ >= enemy_->GetApproachingTimer())
	{
		enemy_->ChangeState(EnemyDevil::kTurn);
	}
}