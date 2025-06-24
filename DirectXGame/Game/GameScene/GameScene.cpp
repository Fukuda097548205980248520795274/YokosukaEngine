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

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get(), directionalLight_.get());

	// 敵の生成と初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(engine_, camera3d_.get(), directionalLight_.get(), Vector3(0.0f, 2.0f, 128.0f));
	enemy_->SetPlayerInstance(player_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	// プレイヤーの更新
	player_->Update();

	// 敵の更新
	if (enemy_)
	{
		enemy_->Update();
	}

	// 衝突判定を行う
	CheckAllCollisions();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// プレイヤーの描画
	player_->Draw();

	// 敵の描画
	if (enemy_)
	{
		enemy_->Draw();
	}
}

/// <summary>
/// 全ての衝突判定を行う
/// </summary>
void GameScene::CheckAllCollisions()
{
	// 判定対象の座標
	Vector3 posA, posB;

	// プレイヤーの弾のリスト
	const std::list<PlayerBullet*>& playerBullets = player_->GetBulletsInstance();

	// 敵の弾のリスト
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBulletsInstance();


#pragma region // 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	for (EnemyBullet* enemyBullet : enemyBullets)
	{
		// 敵弾の座標
		posB = enemyBullet->GetWorldPosition();

		// 球
		Sphere sphereA
		{
			.center = posA,
			.radius = 1.0f
		};

		Sphere sphereB
		{
			.center = posB,
			.radius = 1.0f
		};

		if (IsCollision(sphereA, sphereB))
		{
			player_->OnCollision();
			enemyBullet->OnCollision();
		}
	}

#pragma endregion

#pragma region // 自弾と敵キャラの当たり判定

	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* playerBullet : playerBullets)
	{
		// 自弾の座標
		posB = playerBullet->GetWorldPosition();

		// 球
		Sphere sphereA
		{
			.center = posA,
			.radius = 1.0f
		};

		Sphere sphereB
		{
			.center = posB,
			.radius = 1.0f
		};

		if (IsCollision(sphereA, sphereB))
		{
			enemy_->OnCollision();
			playerBullet->OnCollision();
		}
	}

#pragma endregion

#pragma region // 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets)
	{
		// 自弾の当たり判定
		posA = playerBullet->GetWorldPosition();

		// 球
		Sphere sphereA
		{
			.center = posA,
			.radius = 1.0f
		};

		for (EnemyBullet* enemyBullet : enemyBullets)
		{
			// 敵弾の当たり判定
			posB = enemyBullet->GetWorldPosition();

			// 球
			Sphere sphereB
			{
				.center = posB,
				.radius = 1.0f
			};

			if (IsCollision(sphereA, sphereB))
			{
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}
	
#pragma endregion
}