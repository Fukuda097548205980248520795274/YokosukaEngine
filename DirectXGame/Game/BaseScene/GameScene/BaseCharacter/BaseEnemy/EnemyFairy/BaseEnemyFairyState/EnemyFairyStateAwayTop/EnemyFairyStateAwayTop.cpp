#include "EnemyFairyStateAwayTop.h"
#include "../../EnemyFairy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyFairyStateAwayTop::EnemyFairyStateAwayTop(EnemyFairy* enemy)
	: BaseEnemyFairyState(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();


	// 時間
	time_ = 2.0f;

	// タイマー
	timer_ = 0.0f;


	// 羽ばたきギミックの生成と初期化
	flappingWingR_ = std::make_unique<GimmickFlapping>();
	flappingWingR_->SetGameTimer(enemy_->GetGameTimer());
	flappingWingR_->Initialize(enemy_->GetWingRWorldTransform(), 1.0f);
	flappingWingR_->SetAmplitude(0.4f);
	flappingWingR_->SetRotationAxis(GimmickFlapping::kY);
	flappingWingR_->SetStartRotation(-0.5f);

	flappingWingL_ = std::make_unique<GimmickFlapping>();
	flappingWingL_->SetGameTimer(enemy_->GetGameTimer());
	flappingWingL_->Initialize(enemy_->GetWingLWorldTransform(), -1.0f);
	flappingWingL_->SetAmplitude(0.4f);
	flappingWingL_->SetRotationAxis(GimmickFlapping::kY);
	flappingWingL_->SetStartRotation(0.5f);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyFairyStateAwayTop::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新
	flappingWingR_->Update();
	flappingWingL_->Update();

	// 上に動かす
	worldTransform->translation_.y += 1.0f * (*gameTimer);

	// タイマーを進める
	timer_ += (1.0f / 60.0f) * (*gameTimer);

	// タイマーを越えたら消滅する
	if (timer_ >= time_)
	{
		enemy_->FinisheIt();
	}
}