#include "EnemyJetStateAwayTop.h"
#include "../../EnemyJet.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyJetStateAwayTop::EnemyJetStateAwayTop(EnemyJet* enemy)
	: BaseEnemyJetState(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();


	// 時間
	time_ = 2.0f;

	// タイマー
	timer_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyJetStateAwayTop::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


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