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

	// タイトルシーンの生成と初期化
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize(engine_ , modelHandleStore_.get());
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
			scene_->Initialize(engine_, modelHandleStore_.get());

			break;

		case kGame:
			// ゲーム

			scene_ = std::make_unique<GameScene>();
			scene_->Initialize(engine_, modelHandleStore_.get());
			

			break;
		}

		// フェーズリクエストの消去
		scenePhaseRequest_ = std::nullopt;
	}

	// シーンの更新処理
	scene_->Update();


	// 終了フラグの判定
	if (scene_->IsFinished())
	{
		// 現在のフェーズに合わせて切り替える
		switch (scenePhase_)
		{
		case kTitle:
			// タイトル

			scenePhaseRequest_ = kGame;

			break;

		case kGame:
			// ゲーム

			break;
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{
	// シーンの描画処理
	scene_->Draw();
}