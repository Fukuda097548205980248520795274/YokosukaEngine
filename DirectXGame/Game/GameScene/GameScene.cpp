#include "GameScene.h"

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	// 敵
	for (BaseEnemy* enemy : enemies_)
	{
		delete enemy;
	}
	enemies_.clear();

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
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Broken_Waveforms.mp3");


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get());
	player_->SetGameScene(this);
	mainCamera_->translation_.y = 10.0f;

	// 敵の追加
	EnemyButterfly* enemy = new EnemyButterfly();
	enemy->Initialize(engine_, camera3d_.get(), Vector3(0.0f, 10.0f, 30.0f));
	enemies_.push_back(enemy);


	// ステージオブジェクトの追加
	for (uint32_t i = 0; i < 8; ++i)
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


	// 天球の更新
	skydome_->Update();

	// プレイヤーの更新
	player_->Update();

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


	// ボスの更新
	for (BaseBoss* boss : bosses_)
	{
		boss->Update();
	}


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

	// 敵の描画
	for (BaseEnemy* enemy : enemies_)
	{
		enemy->Draw();
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


	// プレイヤーの弾のリスト
	std::list<BasePlayerBullet*> playerBullets = player_->GetBullets();

	for (BasePlayerBullet* playerBullet : playerBullets)
	{
		segment = playerBullet->GetCollisionSegment();

		for (BaseEnemy* enemy : enemies_)
		{
			aabb = enemy->GetCollisionAABB(enemy->GetWorldPosition());

			if (IsCollision(aabb, segment))
			{
				playerBullet->OnCollision(enemy);
				enemy->OnCollision(playerBullet);
			}
		}
	}
}