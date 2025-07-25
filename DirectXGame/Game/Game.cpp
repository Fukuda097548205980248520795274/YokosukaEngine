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


	// タイトルシーンの生成と初期化
	scene_ = std::make_unique<GameScene>();
	scene_->Initialize(engine_);
}

/// <summary>
/// 更新処理
/// </summary>
void Game::Update()
{
	// 次のフェーズのリクエストがあったとき
	if (scenePhaseRequest_)
	{
		// フェーズ変更
		scenePhase_ = scenePhaseRequest_.value();

		// シーンの初期化
		scene_.release();

		// 切り替えたフェーズの生成と初期化
		switch (scenePhase_)
		{
		case kTitle:
			// タイトル

			scene_ = std::make_unique<TitleScene>();
			scene_->Initialize(engine_);

			break;

		case kGame:
			// ゲーム

			scene_ = std::make_unique<GameScene>();
			scene_->Initialize(engine_);

			break;
		}

		// フェーズリクエストの消去
		scenePhaseRequest_ = std::nullopt;
	}

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