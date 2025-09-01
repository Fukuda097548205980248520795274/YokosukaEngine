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

	// モデルハンドル格納場所の生成と初期化
	modelHandleStore_ = std::make_unique<ModelHandleStore>();
	modelHandleStore_->Initialize(engine_);

	// テクスチャハンドル格納場所の生成と初期化
	textureHandleStore_ = std::make_unique<TextureHandleStore>();
	textureHandleStore_->Initialize(engine_);

	// タイトルシーンの生成と初期化
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize(engine_ , modelHandleStore_.get() , textureHandleStore_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void Game::Update()
{
	// シーンの更新処理
	scene_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{
	// シーンの描画処理
	scene_->Draw();
}