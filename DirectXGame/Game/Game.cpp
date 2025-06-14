#include "Game.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void Game::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert (engine != nullptr);

	// 引数を受け取る
	engine_ = engine;


	/*--------------
	    ゲーム制作
	--------------*/

	// タイトルシーンの生成と初期化
	titleScene_ = std::make_unique<TitleScene>();
	titleScene_->Initialize(engine_);

	// ゲームシーンの生成と初期化
	gameScene_ = std::make_unique<GameScene>();
	gameScene_->Initialize(engine_);
}

/// <summary>
/// 更新処理
/// </summary>
void Game::Update()
{

	/*--------------
	    ゲーム制作
	--------------*/

	// タイトルシーン更新
	titleScene_->Update();

	// ゲームシーン更新
	//gameScene_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{

	/*---------------
	    ゲーム制作
	---------------*/

	// タイトルシーン描画
	titleScene_->Draw();

	// ゲームシーン描画
	//gameScene_->Draw();
}