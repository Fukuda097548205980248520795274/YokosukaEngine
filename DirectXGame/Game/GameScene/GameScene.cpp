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


	// Create and initialize DirectionalLight
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// Create and initialize WorldTransform
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// Create and initialize uvTransform
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// Load the model
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();


	// Suzanne Controls
	ImGui::Begin("Suzanne");
	ImGui::DragFloat3("rotate", &worldTransform_->rotation_.x, 0.01f);
	ImGui::End();
	
	// Update the WorldTransform
	worldTransform_->UpdateWorldMatrix();

	// Update the uvTransform
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// Install DirectionalLight
	engine_->SetDirectionalLight(directionalLight_.get());

	// Draw the model
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}