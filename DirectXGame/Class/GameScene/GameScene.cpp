#include "GameScene.h"

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	// カメラ
	delete camera_;
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize(YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// カメラを初期化する
	camera_ = new Camera3D();
	camera_->Initialize(1280.0f, 720.0f);
	camera_->translation_.z = -5.0f;

	// プレイヤーの生成と初期化
	player_ = new Player();
	player_->Initialize(engine_, camera_);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &camera_->translation_.x, 0.1f);
	ImGui::DragFloat3("rotation", &camera_->rotation_.x, 0.01f);
	ImGui::End();

	// カメラを更新する
	camera_->UpdateMatrix();

	// プレイヤー
	player_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// プレイヤー
	player_->Draw();
}