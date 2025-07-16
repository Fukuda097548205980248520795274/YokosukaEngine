#include "GameScene.h"

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	// プレイヤーの弾
	for (BasePlayerBullet* playerBullet : playerBullets_)
	{
		delete playerBullet;
	}
	playerBullets_.clear();

	// 敵
	for (BaseEnemy* enemy : enemies_)
	{
		delete enemy;
	}
	enemies_.clear();

	// 敵の弾
	for (BaseEnemyBullet* enemyBullet : enemyBullets_)
	{
		delete enemyBullet;
	}
	enemyBullets_.clear();

	// ボス
	for (BaseBoss* boss : bosses_)
	{
		delete boss;
	}
	bosses_.clear();

	// ステージオブジェクト
	for (StageObject* stageObject : stageObjects_)
	{
		delete stageObject;
	}
	stageObjects_.clear();
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

	// サウンドを読み込む
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Addictive_Waveform.mp3");


	// 中心軸の生成と初期化
	centerAxis_ = std::make_unique<CenterAxis>();
	centerAxis_->Initliaze(engine_ , camera3d_.get());
	
	// メインカメラが中心軸を追従する
	mainCamera_->SetPivotPointWorldTransform(centerAxis_->GetWorldTransform());


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLight_->intensity_ = 0.5f;

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());
	skydome_->SetParent(centerAxis_->GetWorldTransform());

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get());
	player_->SetGameScene(this);
	player_->SetParent(mainCamera_->GetWorldTransform());

	// 敵の追加
	for (uint32_t i = 0; i < 3; i++)
	{
		EnemyButterfly* enemy = new EnemyButterfly();
		enemy->Initialize(engine_, camera3d_.get(), Vector3(-15.0f + 15.0f * i, 0.0f, 20.0f), centerAxis_.get(), player_.get(), this);
		enemies_.push_back(enemy);
	}


	// ステージオブジェクトの追加
	for (uint32_t i = 0; i < 12; ++i)
	{
		TutorialGroundEmpty* stageObject = new TutorialGroundEmpty();
		stageObject->Initialize(engine_, camera3d_.get(), Vector3(0.0f, 0.0f, 60.0f * i));

		// リストに追加する
		stageObjects_.push_back(stageObject);
	}

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
	{
		playHandle_ = engine_->PlaySoundData(soundHandle_ , 0.3f);
	}


	// 中心軸の更新
	centerAxis_->Update();

	// 天球の更新
	skydome_->Update();

	// プレイヤーの更新
	player_->Update();

	// プレイヤーの弾の更新
	PlayerBulletUpdate();

	// 敵の更新
	EnemyUpdate();

	// 敵の弾の更新
	EnemyBulletUpdate();

	// ボスの更新
	BossUpdate();

	// ステージオブジェクトの更新
	StageObjectUpdate();


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

	// 天球の描画
	skydome_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// プレイヤーの弾
	for (BasePlayerBullet* playerBullet : playerBullets_)
	{
		playerBullet->Draw();
	}

	// 敵の描画
	for (BaseEnemy* enemy : enemies_)
	{
		enemy->Draw();
	}

	// 敵の弾の描画
	for (BaseEnemyBullet* enemyBullet : enemyBullets_)
	{
		enemyBullet->Draw();
	}

	// ボスの描画
	for (BaseBoss* boss : bosses_)
	{
		boss->Draw();
	}

	// ステージオブジェクトの描画
	for (StageObject* stageObject : stageObjects_)
	{
		stageObject->Draw();
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

	// Scene描画
	Scene::Draw();


	// 中心軸の描画
#ifdef _DEBUG

	//centerAxis_->Draw();

#endif
}


/// <summary>
/// プレイヤーの弾を発射する
/// </summary>
/// <param name="playerBullet"></param>
void GameScene::PlayerBulletShot(BasePlayerBullet* playerBullet)
{
	// nullptrチェック
	assert(playerBullet);

	// リストに追加する
	playerBullets_.push_back(playerBullet);
}

/// <summary>
/// 敵の弾を発射する
/// </summary>
/// <param name="enemyBullet"></param>
void GameScene::EnemyBulletShot(BaseEnemyBullet* enemyBullet)
{
	// nullptrチェック
	assert(enemyBullet);

	// リストに追加する
	enemyBullets_.push_back(enemyBullet);
}



/// <summary>
/// プレイヤーの弾の更新処理
/// </summary>
void GameScene::PlayerBulletUpdate()
{
	// プレイヤーの弾の更新
	for (BasePlayerBullet* playerBullet : playerBullets_)
	{
		playerBullet->Update();
	}

	// 終了したプレイヤーの弾をリストから削除する
	playerBullets_.remove_if([](BasePlayerBullet* playerBullet)
		{
			if (playerBullet->IsFinished())
			{
				delete playerBullet;
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
	for (BaseEnemy* enemy : enemies_)
	{
		enemy->Update();
	}

	// 終了した敵をリストから削除する
	enemies_.remove_if([](BaseEnemy* enemy)
		{
			if (enemy->IsFinished())
			{
				delete enemy;
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
	for (BaseEnemyBullet* enemyBullet : enemyBullets_)
	{
		enemyBullet->Update();
	}

	// 終了した敵の弾をリストから削除する
	enemyBullets_.remove_if([](BaseEnemyBullet* enemyBullet)
		{
			if (enemyBullet->IsFinished())
			{
				delete enemyBullet;
				return true;
			}
			return false;
		}
	);
}

/// <summary>
/// ボスの更新処理
/// </summary>
void GameScene::BossUpdate()
{
	// ボスの更新
	for (BaseBoss* boss : bosses_)
	{
		boss->Update();
	}

	// 終了したボスをリストから削除する
	bosses_.remove_if([](BaseBoss* boss)
		{
			if (boss->IsFinished())
			{
				delete boss;
				return true;
			}
			return false;
		}
	);
}

/// <summary>
/// ステージオブジェクトの更新処理
/// </summary>
void GameScene::StageObjectUpdate()
{
	// ステージオブジェクトの更新
	for (StageObject* stageObject : stageObjects_)
	{
		stageObject->Update();
	}

	// 終了したステージオブジェクトをリストから削除する
	stageObjects_.remove_if([](StageObject* stageObject)
		{
			if (stageObject->IsFinished())
			{
				delete stageObject;
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

	for (BasePlayerBullet* playerBullet : playerBullets_)
	{
		segment = playerBullet->GetCollisionSegment();

		for (BaseEnemy* enemy : enemies_)
		{
			aabb = enemy->GetCollisionAABB(enemy->GetBodyWorldPosition());

			if (IsCollision(aabb, segment))
			{
				playerBullet->OnCollision(enemy);
				enemy->OnCollision(playerBullet);
			}
		}
	}
}