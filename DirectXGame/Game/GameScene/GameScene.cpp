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

	// メインカメラの位置を少しずらす
	mainCamera_->translation_.z = -50.0f;

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get(), directionalLight_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	// プレイヤーの更新
	player_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// プレイヤーの描画
	player_->Draw();
}