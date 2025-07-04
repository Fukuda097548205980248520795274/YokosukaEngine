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


	// Create and initialize directionalLight
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// Create and initialize player
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_ , camera3d_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	// Update player
	player_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// Place directionalLight
	engine_->SetDirectionalLight(directionalLight_.get());

	// Draw the player
	player_->Draw();
}