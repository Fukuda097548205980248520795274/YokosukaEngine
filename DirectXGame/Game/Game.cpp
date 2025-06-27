#include "Game.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void Game::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert (engine);

	// 引数を受け取る
	engine_ = engine;


	// ゲームシーンの生成と初期化
	gameScene_ = std::make_unique<GameScene>();
	gameScene_->Initialize(engine_);
}

/// <summary>
/// 更新処理
/// </summary>
void Game::Update()
{
	// ゲームシーンの更新
	gameScene_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{

	// ゲームシーンの描画
	gameScene_->Draw();
}