#include "EnemyDevilBehaviorShot.h"
#include "../../EnemyDevil.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyDevilBehaviorShot::EnemyDevilBehaviorShot(EnemyDevil* enemy) : BaseEnemyDevilBehavior(enemy)
{
	oneSwingValues_[0] += enemy_->GetBodyWorldTransform()->rotation_.y;
	oneSwingValues_[1] += enemy_->GetBodyWorldTransform()->rotation_.y;

	twoSwingValues_[0] += enemy_->GetBodyWorldTransform()->rotation_.y;
	twoSwingValues_[1] += enemy_->GetBodyWorldTransform()->rotation_.y;

	threeSwingValues_[0] += enemy_->GetBodyWorldTransform()->rotation_.y;
	threeSwingValues_[1] += enemy_->GetBodyWorldTransform()->rotation_.y;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyDevilBehaviorShot::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();

	// 本体のワールドトランスフォーム
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();


	// 発射パラメータを進める
	shotParameter_ += kShotParameterVelocity * (*gameTimer);
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);


	if (shotParameter_ >= oneSwingFrames_[0] && shotParameter_ <= oneSwingFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - oneSwingFrames_[0]) / (oneSwingFrames_[1] - oneSwingFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);
		bodyWorldTransform->rotation_.y = Lerp(oneSwingValues_[0], oneSwingValues_[1], easing);
	}

	if (shotParameter_ >= twoSwingFrames_[0] && shotParameter_ <= twoSwingFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - twoSwingFrames_[0]) / (twoSwingFrames_[1] - twoSwingFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);
		bodyWorldTransform->rotation_.y = Lerp(twoSwingValues_[0], twoSwingValues_[1], easing);
	}

	if (shotParameter_ >= threeSwingFrames_[0] && shotParameter_ <= threeSwingFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - threeSwingFrames_[0]) / (threeSwingFrames_[1] - threeSwingFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);
		bodyWorldTransform->rotation_.y = Lerp(threeSwingValues_[0], threeSwingValues_[1], easing);
	}


	// 発射パラメータが最大値で終了する
	if (shotParameter_ >= kShotParameterMax)
	{
		isFinished_ = true;
		bodyWorldTransform->rotation_.y = oneSwingValues_[0];
	}
}