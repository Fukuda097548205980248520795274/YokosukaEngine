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

	// 最初のシーン初期化
	scene_ = Scene::kTitle;

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

	// シーン状態遷移
	ChangeScene();

	// シーン更新
	UpdateScene();
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{

	// シーン描画
	DrawScene();
}

/// <summary>
/// シーン状態遷移
/// </summary>
void Game::ChangeScene()
{
	switch (scene_)
	{
	case Scene::kTitle:
		// タイトルシーン
		
		// タイトルシーンが終了したら、ゲームシーンに遷移する
		if (titleScene_->IsFinished())
		{
			scene_ = Scene::kGame;

			// タイトルシーンの解放
			titleScene_.release();

			// ゲームシーンの生成と初期化
			gameScene_ = std::make_unique<GameScene>();
			gameScene_->Initialize(engine_);
		}

		break;

	case Scene::kGame:
		// ゲームシーン

		// ゲームシーンが終了したら、タイトルシーンに遷移する
		if (gameScene_->IsFinished())
		{
			scene_ = Scene::kTitle;

			// ゲームシーンの解放
			gameScene_.release();

			// タイトルシーンの生成と初期化
			titleScene_ = std::make_unique<TitleScene>();
			titleScene_->Initialize(engine_);
		}

		break;
	}
}

/// <summary>
/// シーンを更新する
/// </summary>
void Game::UpdateScene()
{
	switch (scene_)
	{
	case Scene::kTitle:
		// タイトルシーン

		titleScene_->Update();

		break;

	case Scene::kGame:
		// ゲームシーン

		gameScene_->Update();

		break;
	}
}

/// <summary>
/// シーンを描画する
/// </summary>
void Game::DrawScene()
{
	switch (scene_)
	{
	case Scene::kTitle:
		// タイトルシーン

		titleScene_->Draw();

		break;

	case Scene::kGame:
		// ゲームシーン

		gameScene_->Draw();

		break;
	}
}