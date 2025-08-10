#include "EnemyButterflyStateAwayTop.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateAwayTop::EnemyButterflyStateAwayTop(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();


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


	// 時間
	time_ = 2.0f;

	// タイマー
	timer_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyStateAwayTop::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新
	gimmickFlappingWingR_->Update();
	gimmickFlappingWingL_->Update();


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