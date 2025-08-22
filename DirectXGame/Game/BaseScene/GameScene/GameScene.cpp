#include "GameScene.h"
#include "../../ModelHandleStore/ModelHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// BaseScene更新
	BaseScene::Initialize(engine , modelHandleStore , textureHandleStore);

	// サウンドを読み込む
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Addictive_Waveform.mp3");


	// ポーズの生成と初期化
	pose_ = std::make_unique<Pose>();
	pose_->Initialize(engine_ , camera3d_);


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLight_->intensity_ = 0.5f;


	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_, modelHandleStore_, Vector3(0.0f, 0.0f, 0.0f), 5);
	player_->SetGameScene(this);
	player_->SetParent(mainCamera_->GetPivotWorldTransform());

	// ゲームタイマーを取得する
	gameTimer_ = player_->GetGameTimer();

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// 中心軸の回転をメインカメラに渡す
	mainCamera_->SetCameraRotate(stage_->GetCenterAxisWorldTransform()->rotation_);

	// Scene更新
	BaseScene::Update();


	// フェーズ切り替え
	switch (phase_)
	{
	case kFadeIn:
	default:
		// フェードイン
		PhaseFadeInUpdate();

		break;

	case kGameOperation:
		// ゲーム操作
		PhaseGameOperation();

		break;

	case kClearMovie:
		// クリアムービー
		PhaseClearMovide();

		break;

	case kClearOperation:
		// クリア操作
		PhaseClearOperation();

		break;

	case kFadeOut:
		// フェードアウト
		PhaseFadeOut();

		break;
	}


	// ポーズ中は処理しない
	if (pose_->IsPose())
		return;


	// プレイヤーの更新
	player_->Update();

	// プレイヤーの弾の更新
	PlayerBulletUpdate();

	// 敵の更新
	EnemyUpdate();

	// 敵の弾の更新
	EnemyBulletUpdate();

	// ダメージパーティクルの更新
	DamageParticleUpdate();


	// 天球の更新が中心軸の位置に移動する
	skydome_->SetPosition(stage_->GetCenterAxisWorldPosition());
	skydome_->Update();

	// 全ての当たり判定を行う
	AllCheckCollision();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// プレイヤーHUDのレイヤー
	screenHandlePlayerHUD_ = engine_->SetOffscreenEffect(kHide);
	player_->DrawHUD();
	engine_->SetOffscreenEffect(kHide);



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

	// ダメージパーティクルの描画
	for (std::unique_ptr<DamageParticle>& damageParticle : damageParticles_)
	{
		damageParticle->Draw();
	}


	// プレイヤーHUDのスクリーンを加算する
	engine_->SetCopyImageBlendMode(kBlendModeScreen);
	engine_->CopyRtvImage(screenHandlePlayerHUD_);
	engine_->SetCopyImageBlendMode(kBlendModeNormal);

	// 走査線
	engine_->SetOffscreenEffect(kScanline);


	// ポーズの描画
	pose_->Draw();

	// Scene描画
	BaseScene::Draw();
}

/// <summary>
/// ステージを生成する
/// </summary>
/// <param name="enemyScriptPass"></param>
/// <param name="stageObjectScriptPass"></param>
void GameScene::CreateStage(const std::string& controlPointScriptPass, 
	const std::string& enemyScriptPass, const std::string& stageObjectScriptPass)
{
	// ステージの生成と初期化
	stage_ = std::make_unique<Stage>();
	stage_->Initialize(engine_, camera3d_, modelHandleStore_, player_->GetGameTimer(), this);
	stage_->LoadControlPointScript(controlPointScriptPass.c_str());
	stage_->SetTarget(player_.get());
	stage_->LoadEnemyScript(enemyScriptPass.c_str());
	stage_->LoadStageObjectScript(stageObjectScriptPass.c_str());


	// 中心軸をメインカメラの親とする
	mainCamera_->SetPivotParent(stage_->GetCenterAxisWorldTransform());

	skydome_->SetPosition(stage_->GetCenterAxisWorldPosition());
}


/// <summary>
/// 敵を出現させる
/// </summary>
/// <param name="enemies"></param>
void GameScene::EnemySummon(std::unique_ptr<BaseEnemy> enemy)
{
	// リストに追加する
	enemies_.push_back(std::move(enemy));
}

/// <summary>
/// プレイヤーの弾を発射する
/// </summary>
/// <param name="playerBullet"></param>
void GameScene::PlayerBulletShot(std::unique_ptr<BasePlayerBullet> playerBullet)
{
	// 中心軸を親にする
	playerBullet->SetParent(stage_->GetCenterAxisWorldTransform());

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
	enemyBullet->SetParent(stage_->GetCenterAxisWorldTransform());

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
/// ダメージパーティクルの更新処理
/// </summary>
void GameScene::DamageParticleUpdate()
{
	// ダメージパーティクルの更新
	for (std::unique_ptr<DamageParticle>& damageParticle : damageParticles_)
	{
		damageParticle->Update();
	}

	// 終了したダメージパーティクルをリストから削除する
	damageParticles_.remove_if([](std::unique_ptr<DamageParticle>& damageParticle)
		{
			if (damageParticle->IsFinished())
			{
				damageParticle.release();
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

				std::unique_ptr<DamageParticle> damageParticle = std::make_unique<DamageParticle>();
				damageParticle->Initialize(engine_, camera3d_ , enemy->GetWorldPosition());
				damageParticles_.push_back(std::move(damageParticle));
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





/// <summary>
/// フェードインの更新処理
/// </summary>
void GameScene::PhaseFadeInUpdate()
{
	// スタートボタンでポーズ
	if (engine_->IsGamepadEnable(0))
	{
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_START))
		{
			pose_->PoseButton();
		}
	}

	// ポーズの更新
	pose_->Update();

	// ポーズ中は音楽を停止する
	if (pose_->IsPose())
	{
		engine_->SetVolume(playHandle_, 0.0f);
		engine_->SetPitch(playHandle_, 0.0f);
		return;
	} else
	{
		// ポーズ中ではないときは、音楽を流す
		if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
		{
			playHandle_ = engine_->PlaySoundData(soundHandle_, 0.3f);
		}

		engine_->SetVolume(playHandle_, 0.3f);
		engine_->SetPitch(playHandle_, pitch_ * (*gameTimer_));
	}


	// パラメータを進める
	fadeInParameter_ += 1.0f / 60.0f;
	fadeInParameter_ = std::min(fadeInParameter_, kFadeInPrameterMax);

	// パラメータ最大でゲーム操作フェーズに移行する
	if (fadeInParameter_ >= kFadeInPrameterMax)
	{
		phase_ = kGameOperation;
	}


	// ステージの更新
	stage_->Update();
}

/// <summary>
/// ゲーム操作の更新処理
/// </summary>
void GameScene::PhaseGameOperation()
{
	// スタートボタンでポーズ
	if (engine_->IsGamepadEnable(0))
	{
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_START))
		{
			pose_->PoseButton();
		}
	}

	// ポーズの更新
	pose_->Update();

	// ポーズ中は音楽を停止する
	if (pose_->IsPose())
	{
		engine_->SetVolume(playHandle_, 0.0f);
		engine_->SetPitch(playHandle_, 0.0f);
		return;
	} else
	{
		// ポーズ中ではないときは、音楽を流す
		if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
		{
			playHandle_ = engine_->PlaySoundData(soundHandle_, 0.3f);
		}

		engine_->SetVolume(playHandle_, 0.3f);
		engine_->SetPitch(playHandle_, pitch_ * (*gameTimer_));
	}



	// ステージの更新
	stage_->Update();

	// 最後までたどり着いたらプレイヤーがゲームクリア状態になる
	if (stage_->IsClear())
	{
		player_->SetIsGameClear(true);

		// BGMを止める
		engine_->StopSound(playHandle_);

		// クリアムービーに移行する
		phase_ = kClearMovie;
	}
}

/// <summary>
/// クリアムービーの更新処理
/// </summary>
void GameScene::PhaseClearMovide()
{
	// パラメータを進める
	clearMovieParameter_ += 1.0f / 60.0f;
	clearMovieParameter_ = std::min(clearMovieParameter_, kClearMoviePrameterMax);

	// パラメータが最大で終了する
	if (clearMovieParameter_ >= kClearMoviePrameterMax)
	{
		phase_ = kClearOperation;
	}
}

/// <summary>
/// クリア操作の更新処理
/// </summary>
void GameScene::PhaseClearOperation()
{
	
}

/// <summary>
/// フェードアウトの更新処理
/// </summary>
void GameScene::PhaseFadeOut()
{
	// パラメータを進める
	fadeOutParameter_ += 1.0f / 60.0f;
	fadeOutParameter_ = std::min(fadeOutParameter_, kFadeOutPrameterMax);

	// パラメータが最大で終了する
	if (fadeOutParameter_ >= kFadeOutPrameterMax)
	{
		isFinished_ = true;
		return;
	}
}