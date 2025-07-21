#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// Scene初期化
	Scene::Initialize(engine);


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());

	// 地面の生成と初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(engine_, camera3d_.get());

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get(), Vector3(0.0f, 0.0f, 0.0f));
	player_->SetMainCamera(mainCamera_.get());

	// ロックオンの生成と初期化
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize(engine_, camera2d_.get());

	// プレイヤーにメインカメラを追従させる
	mainCamera_->SetTarget(player_->GetWorldTransform());

	// 敵を生成する
	for (int i = 0; i < 5; i++)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->Initialize(engine_, camera3d_.get(), Vector3(-20.0f + i * 10.0f, 0.0f, 30.0f));
		enemies_.push_back(std::move(enemy));
	}
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

	// プレイヤー更新
	player_->Update();

	// ロックオンの更新
	lockOn_->Update();

	// 敵の更新
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Update();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// 平行光源の設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// 天球の描画
	skydome_->Draw();

	// 地面の描画
	ground_->Draw();

	// プレイヤー描画
	player_->Draw();

	// ロックオンの描画
	lockOn_->Draw();

	// 敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Draw();
	}
}