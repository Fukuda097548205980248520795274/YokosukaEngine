#include "EnemyBirdStateApproacingRear.h"
#include "../../EnemyBird.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyBirdStateApproacingRear::EnemyBirdStateApproacingRear(EnemyBird* enemy)
	: BaseEnemyBirdState(enemy)
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
	worldTransform->translation_.z += -50.0f;


	// 時間
	time_ = 5.0f;

	// タイマー
	timer_ = 0.0f;

	// 通常ビヘイビアから
	behavior_ = std::make_unique<EnemyBirdBehaviorNormal>(enemy_);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyBirdStateApproacingRear::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// ビヘイビア切り替え
	if (behaviorRequest_)
	{
		// 現在のビヘイビアを切り替える
		currentBehavior_ = behaviorRequest_.value();

		// ビヘイビアを削除する
		behavior_.release();

		switch (currentBehavior_)
		{
		case kNormal:
			// 通常
			behavior_ = std::make_unique<EnemyBirdBehaviorNormal>(enemy_);

			break;

		case kShot:
			// 発射
			behavior_ = std::make_unique<EnemyBirdBehaviorShot>(enemy_);

			break;
		}

		// リクエストを消す
		behaviorRequest_ = std::nullopt;
	}

	// ビヘイビア更新
	behavior_->Update();


	// 終了したら、現在のビヘイビアに合わせて予約する
	if (behavior_->IsFinished())
	{
		switch (currentBehavior_)
		{
		case kNormal:
			// 通常
			behaviorRequest_ = kShot;

			break;

		case kShot:
			// 発射
			behaviorRequest_ = kNormal;

			break;
		}
	}


	// 奥に進む
	worldTransform->translation_ += enemy_->GetApproachingDirection() * (*gameTimer);


	// 通常ビヘイビア時にタイマーを進める
	if (currentBehavior_ == kNormal)
	{
		timer_ += (1.0f / 60.0f) * (*gameTimer);

		// 終了したら、離脱する
		if (timer_ >= enemy_->GetApproachingTimer())
		{
			enemy_->ChangeState(EnemyBird::kAwayTop);
		}
	}
}