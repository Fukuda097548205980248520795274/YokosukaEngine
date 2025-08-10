#include "EnemyBirdBehaviorShot.h"
#include "../../EnemyBird.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyBirdBehaviorShot::EnemyBirdBehaviorShot(EnemyBird* enemy) : BaseEnemyBirdBehavior(enemy)
{
	// 初期値を取得する
	rotateValues[0] = enemy_->GetWorldTransform()->rotation_.z;
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



	// 1回転
	if (shotParameter_ >= rotateFrames[0] && shotParameter_ <= rotateFrames[1])
	{
		// 媒介変数
		float t = (shotParameter_ - rotateFrames[0]) / (rotateFrames[1] - rotateFrames[0]);

		// イージング
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		bodyWorldTransform->rotation_.z = Lerp(rotateValues[0], rotateValues[1], easing);
	}



	// 発射パラメータを進める
	shotParameter_ += kShotParameterVelocity * (*gameTimer);
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);

	// 発射パラメータが最大値で終了する
	if (shotParameter_ >= kShotParameterMax)
	{
		isFinished_ = true;
		bodyWorldTransform->rotation_.z = 0.0f;
	}
}