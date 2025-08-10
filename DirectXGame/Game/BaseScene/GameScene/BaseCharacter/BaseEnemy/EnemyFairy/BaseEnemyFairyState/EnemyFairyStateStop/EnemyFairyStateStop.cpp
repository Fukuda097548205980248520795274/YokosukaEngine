#include "EnemyFairyStateStop.h"
#include "../../EnemyFairy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyFairyStateStop::EnemyFairyStateStop(EnemyFairy* enemy)
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


	// 通常ビヘイビアから
	behavior_ = std::make_unique<EnemyFairyBehaviorNormal>(enemy_);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyFairyStateStop::Update()
{
	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();

	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();


	// 羽ばたきギミックの更新
	flappingWingR_->Update();
	flappingWingL_->Update();

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
			behavior_ = std::make_unique<EnemyFairyBehaviorNormal>(enemy_);

			break;

		case kShot:
			// 発射
			behavior_ = std::make_unique<EnemyFairyBehaviorShot>(enemy_);

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
		timer_ += 1.0f / 60.0f * (*gameTimer);

		// 終了したら、離脱する
		if (timer_ >= time_)
		{
			enemy_->ChangeState(EnemyFairy::kAwayTop);
		}
	}
}