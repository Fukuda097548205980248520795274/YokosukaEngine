#include "EnemyFairyStateTurn.h"
#include "../../EnemyFairy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyFairyStateTurn::EnemyFairyStateTurn(EnemyFairy* enemy)
	: BaseEnemyFairyState(enemy)
{
	// 時間
	time_ = 0.75f;

	// タイマー
	timer_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyFairyStateTurn::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();

	float currentTheta = worldTransform->rotation_.y;
	float goalTheta = std::numbers::pi_v<float>;


	// 角度の差
	float diff = goalTheta - currentTheta;

	diff = std::fmod(diff, std::numbers::pi_v<float> *2.0f);

	if (diff > std::numbers::pi_v<float>)
	{
		diff += -2.0f * std::numbers::pi_v<float>;
	} else if (diff < -std::numbers::pi_v<float>)
	{
		diff += 2.0f * std::numbers::pi_v<float>;
	}


	// 回転する
	worldTransform->rotation_.y = Lerp(currentTheta, currentTheta + diff, 0.1f * (*gameTimer));

	// タイマーを進める
	timer_ += (1.0f / 60.0f) * (*gameTimer);

	// タイマーを越えたら停止状態に移行する
	if (timer_ >= time_)
	{
		enemy_->ChangeState(EnemyFairy::kStop);
	}
}