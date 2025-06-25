#include "GameScene.h"

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	// プレイヤーの弾
	for (PlayerBullet* playerBullet : playerBullets_)
	{
		delete playerBullet;
	}
	playerBullets_.clear();

	// 敵
	for (Enemy* enemy : enemies_)
	{
		delete enemy;
	}
	enemies_.clear();

	// 敵の弾
	for (EnemyBullet* enemyBullet : enemyBullets_)
	{
		delete enemyBullet;
	}
	enemyBullets_.clear();
}

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
	player_->Initialize(engine_, camera3d_.get(), directionalLight_.get(), Vector3(0.0f, 0.0f, 50.0f));
	player_->SetGameSceneInstance(this);

	// 敵の生成と初期化と登録
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(engine_, camera3d_.get(), directionalLight_.get(), Vector3(0.0f, 2.0f, 50.0f));
	newEnemy->SetGameSceneInstance(this);
	newEnemy->SetPlayerInstance(player_.get());
	enemies_.push_back(newEnemy);

	// メインカメラを親にする
	player_->SetWorldTransformParent(mainCamera_->GetWorldTransform());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();


	// プレイヤーの更新
	player_->Update();


	// 消滅したプレイヤーの弾をリストから削除する
	playerBullets_.remove_if([](PlayerBullet* playerBullet)
		{
			if (playerBullet->IsFinished())
			{
				delete playerBullet;
				return true;
			}
			return false;
		}
	);

	// プレイヤーの弾の更新
	for (PlayerBullet* playerBullet : playerBullets_)
	{
		playerBullet->Update();
	}


	// 消滅した敵をリストから削除する
	enemies_.remove_if([](Enemy* enemy)
		{
			if (enemy->IsFinished())
			{
				delete enemy;
				return true;
			}
			return false;
		}
	);
		
	// 敵の更新
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}


	// 消滅した敵の弾をリストから削除する
	enemyBullets_.remove_if([](EnemyBullet* enemyBullet)
		{
			if (enemyBullet->IsFinished())
			{
				delete enemyBullet;
				return true;
			}
			return false;
		}
	);

	// 敵の弾の更新
	for (EnemyBullet* enemyBullet : enemyBullets_)
	{
		enemyBullet->Update();
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

	// プレイヤーの弾の描画
	for (PlayerBullet* playerBullet : playerBullets_)
	{
		playerBullet->Draw();
	}


	// 敵の描画
	for (Enemy* enemy : enemies_)
	{
		enemy->Draw();
	}

	// 敵の弾の描画
	for (EnemyBullet* enemyBullet : enemyBullets_)
	{
		enemyBullet->Draw();
	}
}


/// <summary>
/// 敵の弾をリストに登録する
/// </summary>
/// <param name="enemyBullet"></param>
void GameScene::PushEnemyBullet(EnemyBullet* enemyBullet)
{
	enemyBullets_.push_back(enemyBullet);
}

/// <summary>
/// プレイヤーの弾をリストに登録する
/// </summary>
/// <param name="playerBullet"></param>
void GameScene::PushPlayerBullet(PlayerBullet* playerBullet)
{
	playerBullets_.push_back(playerBullet);
}


/// <summary>
/// コライダーを登録する
/// </summary>
void GameScene::PushCollider()
{

	collisionManager_->PushCollider(player_.get());

	for (PlayerBullet* playerBullet : playerBullets_)
		collisionManager_->PushCollider(playerBullet);

	for (Enemy* enemy : enemies_)
		collisionManager_->PushCollider(enemy);

	for (EnemyBullet* enemyBullet : enemyBullets_)
		collisionManager_->PushCollider(enemyBullet);
}