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
/// <param name="camera3d"></param>
void GameScene::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// マップチップフィールドの生成と初期化
	mapChipField_ = std::make_unique<MapChipField>();
	mapChipField_->LoadMapChipCsv("./Resources/MapChip/blocks.csv");


	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_);

	// ブロックの生成と初期化
	blocks_ = std::make_unique<Blocks>();
	blocks_->Initialize(engine_ ,mapChipField_.get(), camera3d_, directionalLight_.get());

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	Vector3 playerPosition = mapChipField_->GetMapCihpPositionByIndex(1, 18);
	player_->Initialize(engine_, camera3d_, playerPosition, directionalLight_.get());
	player_->SetMapChipField(mapChipField_.get());

	// デスパーティクルの生成と初期化
	deathParticle_ = std::make_unique<DeathParticle>();
	deathParticle_->Initialize(engine_, camera3d_, mapChipField_->GetMapCihpPositionByIndex(3, 18));

	// 敵の生成と初期化
	for (uint32_t i = 0; i < 3; ++i)
	{
		// 生成と初期化
		Enemy* enemy = new Enemy();
		enemy->Initialize(engine_, camera3d_, directionalLight_.get(), mapChipField_->GetMapCihpPositionByIndex(10 + i * 5, 18 - i * 3));

		// リストに登録する
		enemies_.push_back(enemy);
	}
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// 天球を更新する
	skydome_->Update();

	// ブロックを更新する
	blocks_->Update();

	// プレイヤーを更新する
	player_->Update();

	if (deathParticle_)
	{
		// デスパーティクルを更新する
		deathParticle_->Update();
	}

	// 敵を更新する
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}

	// 当たり判定
	CheckAllCollisions();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{

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