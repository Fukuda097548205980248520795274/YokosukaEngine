#include "GameScene.h"

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	// 敵のリスト
	for (Enemy* enemy : enemies_)
	{
		delete enemy;
	}
	enemies_.clear();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;


	// 3Dカメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// メインカメラの生成と初期化
	mainCamera_ = std::make_unique<MainCamera>();
	mainCamera_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// デバッグカメラの表示の初期化
#ifdef _DEBUG

	// 軸方向表示の生成と初期化
	axis_ = std::make_unique<Axis>();
	axis_->Initialize(engine_);

#endif



	// フェーズを初期化する
	phase_ = Phase::kFadeOut;

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// マップチップフィールドの生成と初期化
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("./Resources/MapChip/blocks.csv");


	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());


	// ブロックの生成と初期化
	blocks_ = std::make_unique<Blocks>();
	blocks_->Initialize(engine_ ,mapChipField_.get(), camera3d_.get(), directionalLight_.get());


	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	Vector3 playerPosition = mapChipField_->GetMapCihpPositionByIndex(1, 18);
	player_->Initialize(engine_, camera3d_.get(), playerPosition, directionalLight_.get());
	player_->SetMapChipField(mapChipField_.get());

	// ゲームカメラの追従対象を設定する
	mainCamera_->SetTarget(player_.get());
	mainCamera_->Reset();

	// ゲームカメラの移動範囲
	mainCamera_->SetMovableArea(MainCamera::Rect{ 15.0f , 100.0f , 8.0f , 100.0f });


	// 敵の生成と初期化
	for (uint32_t i = 0; i < 3; ++i)
	{
		// 生成と初期化
		Enemy* enemy = new Enemy();
		enemy->Initialize(engine_, camera3d_.get(), directionalLight_.get(), mapChipField_->GetMapCihpPositionByIndex(10 + i * 5, 18 - i * 3));

		// リストに登録する
		enemies_.push_back(enemy);
	}


	// フェードの生成と初期化
	fade_ = std::make_unique<Fade>();
	fade_->Initialize(engine_);

	// フェードアウト
	fade_->Start(Fade::Status::kFadeOut, kFadeTime);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// ゲームカメラ・デバッグカメラ切り替え
#ifdef _DEBUG

	// Pキーで、カメラを切り替える
	if (engine_->GetKeyTrigger(DIK_P))
	{
		if (isDebugCameraActive_ == false)
		{
			isDebugCameraActive_ = true;
		} else
		{
			isDebugCameraActive_ = false;
		}
	}

	// 軸方向表示の更新
	axis_->Update(camera3d_->rotation_);

#endif

	// カメラの値を渡して更新　ゲームカメラ
	if (isDebugCameraActive_ == false)
	{
		// ゲームカメラの更新
		mainCamera_->Update();
		camera3d_->UpdateOthersCameraData(mainCamera_->GetGameCameraInstance());
	}
	else
	{
		// デバッグカメラの更新
		engine_->DebugCameraUpdate();
		camera3d_->UpdateDebugCameraData(engine_->GetDebugCameraInstance());
	}


	// フェーズ切り替え
	ChangePhase();

	// フェーズの更新処理
	switch (phase_)
	{
	case Phase::kPlay:
		// ゲームプレイ

		GamePlayUpdate();

		break;

	case Phase::kDeath:
		// デス演出

		DeathUpdate();

		break;
	}

	// フェードの更新
	fade_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// デバッグカメラの表示
#ifdef _DEBUG

	// 軸方向表示の描画
	axis_->Draw();

#endif

	// 天球を描画する
	skydome_->Draw();

	// ブロックを描画する
	blocks_->Draw();

	// プレイヤーを描画する
	player_->Draw();

	if (deathParticle_)
	{
		// デスパーティクルを描画する
		deathParticle_->Draw();
	}

	// 敵を描画する
	for (Enemy* enemy : enemies_)
	{
		enemy->Draw();
	}

	// フェードの描画
	fade_->Draw();
}


/// <summary>
/// 全ての当たり判定を行う
/// </summary>
void GameScene::CheckAllCollisions()
{
	// プレイヤーと敵の当たり判定
	CheckPlayerAndEnemyCollision();
}

/// <summary>
/// プレイヤーと敵の当たり判定
/// </summary>
void GameScene::CheckPlayerAndEnemyCollision()
{
	// プレイヤーのAABB
	AABB aabbPlayer = player_->GetAABB();

	// 敵のAABB
	AABB aabbEnemy;

	for (Enemy* enemy : enemies_)
	{
		aabbEnemy = enemy->GetAABB();

		// AABB同士の当たり判定
		if (IsCollision(aabbPlayer, aabbEnemy))
		{
			player_->OnCollision(enemy);
			enemy->OnCollision(player_.get());
		}
	}
}

/// <summary>
/// フェーズを切り替える
/// </summary>
void GameScene::ChangePhase()
{
	switch (phase_)
	{
	case Phase::kFadeOut:
		// フェードアウト

		// フェードが終了したら、ゲームプレイに遷移する
		if (fade_->IsFinished())
		{
			phase_ = Phase::kPlay;
		}

		break;

	case Phase::kPlay:
		// ゲームプレイ

		if (player_->IsDead())
		{
			// 切り替え
			phase_ = Phase::kDeath;

			// プレイヤーの座標を取得
			const Vector3& playerPosition = player_->GetWorldPosition();

			// デスパーティクルを発生
			deathParticle_ = std::make_unique<DeathParticle>();
			deathParticle_->Initialize(engine_, camera3d_.get(), playerPosition);
		}
		
		break;

	case Phase::kDeath:
		// デス演出

		// デスパーティクルが消滅したらフェードイン
		if (deathParticle_)
		{
			if (deathParticle_->IsFinished())
			{
				phase_ = Phase::kFadeIn;
				fade_->Start(Fade::Status::kFadeIn, kFadeTime);
			}
		}

		break;

	case Phase::kFadeIn:
		// フェードイン

		// フェードが終了したら、ゲームシーンを終了する
		if (fade_->IsFinished())
		{
			isFinished_ = true;
		}

		break;
	}
}


/// <summary>
/// ゲームプレイの更新処理
/// </summary>
void GameScene::GamePlayUpdate()
{
	// 天球を更新する
	skydome_->Update();

	// ブロックを更新する
	blocks_->Update();

	// プレイヤーを更新する
	player_->Update();

	// 敵を更新する
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}

	// 当たり判定
	CheckAllCollisions();
}

/// <summary>
/// デス演出の更新処理
/// </summary>
void GameScene::DeathUpdate()
{
	// 天球を更新する
	skydome_->Update();

	// ブロックを更新する
	blocks_->Update();

	// デスパーティクルを更新する
	if (deathParticle_)
	{
		deathParticle_->Update();
	}

	// 敵を更新する
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}
}