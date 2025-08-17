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
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize(engine_ , modelHandleStore_.get() , textureHandleStore_.get());

	scenePhase_ = kTitle;
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
			scene_->Initialize(engine_, modelHandleStore_.get(), textureHandleStore_.get());

			break;

		case kStageSelect:
			// ステージセレクト

			scene_ = std::make_unique<StageSelectScene>();
			scene_->Initialize(engine_, modelHandleStore_.get(), textureHandleStore_.get());

			break;

		case kGame:
			// ゲーム

			scene_ = std::make_unique<GameScene>();
			scene_->Initialize(engine_, modelHandleStore_.get(), textureHandleStore_.get());
			scene_->CreateStage(controlPointScript_,enemyScript_, stageObjectScript_);

			break;

		case kClear:
			// クリア

			scene_ = std::make_unique<ClearScene>();
			scene_->Initialize(engine_ , modelHandleStore_.get(), textureHandleStore_.get());

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

			scenePhaseRequest_ = kStageSelect;

			break;

		case kStageSelect:
			// ステージセレクト

			scenePhaseRequest_ = kGame;

			// パスを取得する
			controlPointScript_ = scene_->GetControlPointScriptPass();
			enemyScript_ = scene_->GetEnemyScriptPass();
			stageObjectScript_ = scene_->GetStageObjectScriptPass();

			break;

		case kGame:
			// ゲーム

			scenePhaseRequest_ = kTitle;

			break;

		case kClear:
			// クリア

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