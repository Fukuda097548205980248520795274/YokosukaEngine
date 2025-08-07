#include "EnemyButterflyStateStop.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateStop::EnemyButterflyStateStop(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy) 
{
	// 通常ビヘイビアから始める
	behavior_ = std::make_unique<EnemyButterflyBehaviorNormal>(enemy);


	// 浮遊ギミック初期化
	gimmickFloating_ = std::make_unique<GimmickFloating>();
	gimmickFloating_->SetGameTimer(enemy_->GetGameTimer());
	gimmickFloating_->Initialize(enemy_->GetBodyWorldTransform(), 0.075f);
	gimmickFloating_->SetAmplitude(0.25f);


	// タイマー
	timer_ = 0.0f;

	// 時間
	time_ = 5.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyStateStop::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();


	// 浮遊ギミックの更新
	gimmickFloating_->Update();


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
			behavior_ = std::make_unique<EnemyButterflyBehaviorNormal>(enemy_);

			break;

		case kShot:
			// 発射
			behavior_ = std::make_unique<EnemyButterflyBehaviorShot>(enemy_);

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

	// 通常ビヘイビア時にタイマーを進める
	if (currentBehavior_ == kNormal)
	{
		timer_ += 1.0f / 60.0f;

		// 終了したら、離脱する
		if (timer_ >= time_)
		{
			enemy_->ChangeState(EnemyButterfly::kAwayTop);
		}
	}
}