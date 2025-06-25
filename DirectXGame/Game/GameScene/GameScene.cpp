#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// Scene更新
	Scene::Initialize(engine);

	// メインカメラの位置を少しずらす
	mainCamera_->translation_.z = -50.0f;

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());

	// 地面の生成と初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(engine_ , camera3d_.get());

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get(), directionalLight_.get(), Vector3(0.0f, -2.0f, 40.0f));

	// メインカメラを親にする
	player_->SetWorldTransformParent(mainCamera_->GetWorldTransform());

	// 敵の生成と初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(engine_, camera3d_.get(), directionalLight_.get(), Vector3(0.0f, 2.0f, 128.0f));
	enemy_->SetPlayerInstance(player_.get());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// カメラを回転させる
	mainCamera_->rotation_.y += 0.001f;

	// Scene更新
	Scene::Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	// プレイヤーの更新
	player_->Update();

	// 敵の更新
	if (enemy_)
	{
		enemy_->Update();
	}


	// 衝突マネージャをクリアする
	collisionManager_->Clear();

	// コライダーを登録する
	PushCollider();

	// 全ての衝突判定を行う
	collisionManager_->CheckAllCollision();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// 天球の描画
	skydome_->Draw();

	// 地面の描画
	ground_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// 敵の描画
	if (enemy_)
	{
		enemy_->Draw();
	}
}

/// <summary>
/// コライダーを登録する
/// </summary>
void GameScene::PushCollider()
{
	// 弾のリスト
	std::list<PlayerBullet*> playerBullets_ = player_->GetBulletsInstance();
	std::list<EnemyBullet*> enemyBullets_ = enemy_->GetBulletsInstance();

	collisionManager_->PushCollider(player_.get());
	collisionManager_->PushCollider(enemy_.get());
	for (PlayerBullet* playerBullet : playerBullets_)
		collisionManager_->PushCollider(playerBullet);
	for (EnemyBullet* enemyBullet : enemyBullets_)
		collisionManager_->PushCollider(enemyBullet);
}