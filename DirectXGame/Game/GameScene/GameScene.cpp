#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void GameScene::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_, directionalLight_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// プレイヤーを更新する
	player_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// プレイヤーを描画する
	player_->Draw();
}