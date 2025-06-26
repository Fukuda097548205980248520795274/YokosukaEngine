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
	player_->Initialize(engine_, camera3d_.get() , camera2d_.get(), directionalLight_.get(), Vector3(0.0f, 0.0f, 50.0f));
	player_->SetGameSceneInstance(this);

	// メインカメラを親にする
	player_->SetWorldTransformParent(mainCamera_->GetWorldTransform());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();

	// 敵発生データを読み込む
	LoadEnemyPopData();
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


	// 敵発生コマンドの更新
	UpdateEnemyPopCommands();

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

/// <summary>
/// 敵発生データの読み込み
/// </summary>
void GameScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

/// <summary>
/// 敵発生コマンドの更新処理
/// </summary>
void GameScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWait_)
	{
		// 待機タイマーを進める
		waitTimer_--;

		if (waitTimer_ <= 0)
		{
			// 待機完了
			isWait_ = false;
		}

		return;
	}


	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(enemyPopCommands_, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 1単語
		std::string word;

		// ,区切りで行の先頭文字を取得
		std::getline(line_stream, word, ',');


		// "//"で始まる行はコメント
		if (word.find("//") == 0)
			continue;

		// "POP"コマンド
		if (word.find("POP") == 0)
		{
			// x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			SummonEnemy(Vector3(x, y, z));
		}

		// "WAIT"コマンド
		if (word.find("WAIT") == 0)
		{
			std::getline(line_stream, word, ',');

			// 待ち時間
			uint32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			break;
		}
	}
}

/// <summary>
/// 敵を発生させる
/// </summary>
/// <param name="posision"></param>
void GameScene::SummonEnemy(const Vector3& posision)
{
	// 敵の生成と初期化と登録
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(engine_, camera3d_.get(), directionalLight_.get(), posision);

	newEnemy->SetGameSceneInstance(this);
	newEnemy->SetPlayerInstance(player_.get());

	enemies_.push_back(newEnemy);
}