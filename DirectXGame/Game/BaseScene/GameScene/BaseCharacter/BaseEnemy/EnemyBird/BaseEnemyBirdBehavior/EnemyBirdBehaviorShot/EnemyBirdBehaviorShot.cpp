#include "EnemyBirdBehaviorShot.h"
#include "../../EnemyBird.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyBirdBehaviorShot::EnemyBirdBehaviorShot(EnemyBird* enemy) : BaseEnemyBirdBehavior(enemy)
{
	// 初期値を取得する
	topWingRInclinationValues_[0] += enemy_->GetTopWingRWorldTransform()->rotation_.z;
	topWingRInclinationValues_[1] += enemy_->GetTopWingRWorldTransform()->rotation_.z;

	topWingLInclinationValues_[0] += enemy_->GetTopWingLWorldTransform()->rotation_.z;
	topWingLInclinationValues_[1] += enemy_->GetTopWingLWorldTransform()->rotation_.z;

	topWingROneSwingInclinationValues_[0] += enemy_->GetTopWingRWorldTransform()->rotation_.z;
	topWingROneSwingInclinationValues_[1] += enemy_->GetTopWingRWorldTransform()->rotation_.z;

	topWingLOneSwingInclinationValues_[0] += enemy_->GetTopWingLWorldTransform()->rotation_.z;
	topWingLOneSwingInclinationValues_[1] += enemy_->GetTopWingLWorldTransform()->rotation_.z;

	rotateValues[0] += enemy_->GetWorldTransform()->rotation_.z;
	rotateValues[1] += enemy_->GetWorldTransform()->rotation_.z;


	topWingRReturnValues_[0] += enemy_->GetTopWingRWorldTransform()->rotation_.z;
	topWingRReturnValues_[1] += enemy_->GetTopWingRWorldTransform()->rotation_.z;

	topWingLReturnValues_[0] += enemy_->GetTopWingLWorldTransform()->rotation_.z;
	topWingLReturnValues_[1] += enemy_->GetTopWingLWorldTransform()->rotation_.z;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyBirdBehaviorShot::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();

	// 本体のワールドトランスフォーム
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();

	// 羽のワールドトランスフォーム
	WorldTransform* topWingRWorldTransform = enemy_->GetTopWingRWorldTransform();
	WorldTransform* topWingLWorldTransform = enemy_->GetTopWingLWorldTransform();

	// 傾き
	if (shotParameter_ >= wingInclinationFrames_[0] && shotParameter_ <= wingInclinationFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - wingInclinationFrames_[0]) / (wingInclinationFrames_[1] - wingInclinationFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		topWingRWorldTransform->rotation_.z = Lerp(topWingRInclinationValues_[0], topWingRInclinationValues_[1], easing);
		topWingLWorldTransform->rotation_.z = Lerp(topWingLInclinationValues_[0], topWingLInclinationValues_[1], easing);
	}

	// 一回転傾き
	if (shotParameter_ >= oneSwingInclinationFrames_[0] && shotParameter_ <= oneSwingInclinationFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - oneSwingInclinationFrames_[0]) / (oneSwingInclinationFrames_[1] - oneSwingInclinationFrames_[0]);
		float easing = t;

		topWingRWorldTransform->rotation_.z = Lerp(topWingROneSwingInclinationValues_[0], topWingROneSwingInclinationValues_[1], easing);
		topWingLWorldTransform->rotation_.z = Lerp(topWingLOneSwingInclinationValues_[0], topWingLOneSwingInclinationValues_[1], easing);
	}

	// 1回転
	if (shotParameter_ >= rotateFrames[0] && shotParameter_ <= rotateFrames[1])
	{
		// 媒介変数
		float t = (shotParameter_ - rotateFrames[0]) / (rotateFrames[1] - rotateFrames[0]);
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		bodyWorldTransform->rotation_.z = Lerp(rotateValues[0], rotateValues[1], easing);
	}

	// 元に戻す
	if (shotParameter_ >= returnFrames_[0] && shotParameter_ <= returnFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - returnFrames_[0]) / (returnFrames_[1] - returnFrames_[0]);
		float easing = t;

		topWingRWorldTransform->rotation_.z = Lerp(topWingRReturnValues_[0], topWingRReturnValues_[1], easing);
		topWingLWorldTransform->rotation_.z = Lerp(topWingLReturnValues_[0], topWingLReturnValues_[1], easing);
	}



	// 発射パラメータを進める
	shotParameter_ += kShotParameterVelocity * (*gameTimer);
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);

	// 発射パラメータが最大値で終了する
	if (shotParameter_ >= kShotParameterMax)
	{
		isFinished_ = true;

		bodyWorldTransform->rotation_.z = rotateValues[0];
		topWingRWorldTransform->rotation_.z = topWingRInclinationValues_[0];
		topWingLWorldTransform->rotation_.z = topWingLInclinationValues_[0];
	}
}