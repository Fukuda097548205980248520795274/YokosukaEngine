#include "EnemyButterflyStateStop.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyStateStop::EnemyButterflyStateStop(EnemyButterfly* enemy)
	: BaseEnemyButterflyState(enemy) 
{
	behavior_ = std::make_unique<EnemyButterflyBehaviorNormal>(enemy);


	// 浮遊ギミック初期化
	gimmickFloating_ = std::make_unique<GimmickFloating>();
	gimmickFloating_->Initialize(enemy_->GetBodyWorldTransform(), 0.075f);
	gimmickFloating_->SetAmplitude(0.25f);
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
}