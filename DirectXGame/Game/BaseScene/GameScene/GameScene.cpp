#include "GameScene.h"
#include "../../ModelHandleStore/ModelHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore)
{
	// BaseScene更新
	BaseScene::Initialize(engine , modelHandleStore);

	// サウンドを読み込む
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Addictive_Waveform.mp3");


	// ポーズの生成と初期化
	pose_ = std::make_unique<Pose>();
	pose_->Initialize(engine_);


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLight_->intensity_ = 0.5f;


	// 中心軸の生成と初期化
	centerAxis_ = std::make_unique<CenterAxis>();
	centerAxis_->Initliaze(engine_ , camera3d_);

	// 中心軸をメインカメラの親とする
	mainCamera_->SetPivotParent(centerAxis_->GetWorldTransform());


	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_, modelHandleStore_, Vector3(0.0f, 0.0f, 0.0f), 100);
	player_->SetGameScene(this);
	player_->SetParent(mainCamera_->GetPivotWorldTransform());

	// ゲームタイマーを取得する
	gameTimer_ = player_->GetGameTimer();


	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_);
	skydome_->SetPosition(centerAxis_->GetWorldPosition());

	// 敵の生成と初期化
	for (uint32_t i = 0; i < 3; i++)
	{
		std::unique_ptr<EnemyButterfly> enemyButterfly = std::make_unique<EnemyButterfly>();
		enemyButterfly->SetGameTimer(gameTimer_);
		enemyButterfly->Initialize(engine_, camera3d_, modelHandleStore_, Vector3(-10.0f + i * 10.0f, 0.0f, 25.0f), 50);
		enemyButterfly->SetParent(centerAxis_->GetWorldTransform());
		enemyButterfly->SetTarget(player_.get());
		enemyButterfly->SetGameScene(this);

		// 登録する
		enemies_.push_back(std::move(enemyButterfly));
	}


	// ステージの生成と初期化
	stage_ = std::make_unique<Stage>();
	stage_->Initialize(engine_, camera3d_, modelHandleStore_, player_->GetGameTimer());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// 中心軸の回転をメインカメラに渡す
	mainCamera_->SetCameraRotate(centerAxis_->GetWorldTransform()->rotation_);

	// Scene更新
	BaseScene::Update();


	if (engine_->IsGamepadEnable(0))
	{
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_START))
		{
			pose_->PoseButton();
		}
	}

	pose_->Update();

	if (pose_->IsPose())
		return;



	if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
	{
		playHandle_ = engine_->PlaySoundData(soundHandle_ , 0.3f);
	}

	engine_->SetPitch(playHandle_, pitch_ * (*gameTimer_));


	// ステージの更新
	stage_->Update();

	// 中心軸の更新
	centerAxis_->Update();

	// 天球の更新が中心軸の位置に移動する
	skydome_->SetPosition(centerAxis_->GetWorldPosition());
	skydome_->Update();

	// プレイヤーの更新
	player_->Update();

	// プレイヤーの弾の更新
	PlayerBulletUpdate();

	// 敵の更新
	EnemyUpdate();

	// 敵の弾の更新
	EnemyBulletUpdate();


	// 全ての当たり判定を行う
	AllCheckCollision();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// 平行光源の設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// ステージの描画
	stage_->Draw();

	// 天球の描画
	skydome_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// プレイヤーの弾
	for (std::unique_ptr<BasePlayerBullet>& playerBullet : playerBullets_)
	{
		playerBullet->Draw();
	}

	// 敵の描画
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_)
	{
		enemy->Draw();
	}

	// 敵の弾の描画
	for (std::unique_ptr<BaseEnemyBullet>& enemyBullet : enemyBullets_)
	{
		enemyBullet->Draw();
	}


	// 高輝度抽出
	engine_->SetOffscreenEffect(kBrightnessExtraction);

	// ガウシアンフィルター *3
	engine_->SetOffscreenEffect(kGaussianFilter);
	engine_->SetOffscreenEffect(kGaussianFilter);
	engine_->SetOffscreenEffect(kGaussianFilter);

	// スクリーンコピー
	screenHandleGrow_ = engine_->SetOffscreenEffect(kCopyImage);


	// レイヤーを隠す
	engine_->SetOffscreenEffect(kHide);

	// 第一レイヤー描画
	engine_->CopyRtvImage(0);

	// 高輝度抽出 * ガウシアンフィルター　加算合成
	engine_->SetCopyImageBlendMode(kBlendModeAdd);
	engine_->CopyRtvImage(screenHandleGrow_);
	engine_->SetCopyImageBlendMode(kBlendModeNormal);

	// ポーズの描画
	pose_->Draw();

	// Scene描画
	BaseScene::Draw();


	// 中心軸の描画
	centerAxis_->Draw();
}


/// <summary>
/// プレイヤーの弾を発射する
/// </summary>
/// <param name="playerBullet"></param>
void GameScene::PlayerBulletShot(std::unique_ptr<BasePlayerBullet> playerBullet)
{
	// 中心軸を親にする
	playerBullet->SetParent(centerAxis_->GetWorldTransform());

	// リストに追加する
	playerBullets_.push_back(std::move(playerBullet));
}

/// <summary>
/// 敵の弾を発射する
/// </summary>
/// <param name="enemyBullet"></param>
void GameScene::EnemyBulletShot(std::unique_ptr<BaseEnemyBullet> enemyBullet)
{
	// 中心軸を親にする
	enemyBullet->SetParent(centerAxis_->GetWorldTransform());

	// リストに追加する
	enemyBullets_.push_back(std::move(enemyBullet));
}



/// <summary>
/// プレイヤーの弾の更新処理
/// </summary>
void GameScene::PlayerBulletUpdate()
{
	// プレイヤーの弾の更新
	for (std::unique_ptr<BasePlayerBullet>& playerBullet : playerBullets_)
	{
		playerBullet->Update();
	}

	// 終了したプレイヤーの弾をリストから削除する
	playerBullets_.remove_if([](std::unique_ptr<BasePlayerBullet>& playerBullet)
		{
			if (playerBullet->IsFinished())
			{
				playerBullet.release();
				return true;
			}
			return false;
		}
	);
}

/// <summary>
/// 敵の更新処理
/// </summary>
void GameScene::EnemyUpdate()
{
	// 敵の更新
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_)
	{
		enemy->Update();
	}

	// 終了した敵をリストから削除する
	enemies_.remove_if([](std::unique_ptr<BaseEnemy>& enemy)
		{
			if (enemy->IsFinished())
			{
				enemy.release();
				return true;
			}
			return false;
		}
	);
}

/// <summary>
/// 敵の弾の更新処理
/// </summary>
void GameScene::EnemyBulletUpdate()
{
	// 敵の弾の更新
	for (std::unique_ptr<BaseEnemyBullet>& enemyBullet : enemyBullets_)
	{
		enemyBullet->Update();
	}

	// 終了した敵の弾をリストから削除する
	enemyBullets_.remove_if([](std::unique_ptr<BaseEnemyBullet>& enemyBullet)
		{
			if (enemyBullet->IsFinished())
			{
				delete enemyBullet.release();
				return true;
			}
			return false;
		}
	);
}


/// <summary>
/// 全ての当たり判定を行う
/// </summary>
void GameScene::AllCheckCollision()
{
	// 線分
	Segment segment;

	// AABB
	AABB aabb;

	// プレイヤーの弾 敵
	for (std::unique_ptr<BasePlayerBullet>& playerBullet : playerBullets_)
	{
		segment = playerBullet->GetCollisionSegment();

		for (std::unique_ptr<BaseEnemy>& enemy : enemies_)
		{
			aabb = enemy->GetCollisionAABB();

			if (IsCollision(aabb, segment))
			{
				playerBullet->OnCollision(enemy.get());
				enemy->OnCollision(playerBullet.get());
			}
		}
	}


	aabb = player_->GetCollisionAABB();

	// プレイヤー 敵の弾
	for (std::unique_ptr<BaseEnemyBullet>& enemyBullet : enemyBullets_)
	{
		segment = enemyBullet->GetCollisionSegment();
		
		if (IsCollision(aabb, segment))
		{
			player_->OnCollision(enemyBullet.get());
			enemyBullet->OnCollision(player_.get());
		}
	}

	// プレイヤー 敵
	for (std::unique_ptr<BaseEnemy>& enemy : enemies_)
	{
		// 敵が攻撃時当たり判定をする
		if (enemy->IsAttack() == false)
			continue;

		if (IsCollision(aabb, enemy->GetCollisionAABB()))
		{
			player_->OnCollision(enemy.get());
		}
	}
}