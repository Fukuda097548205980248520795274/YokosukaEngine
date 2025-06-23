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


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルハンドル
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");

	// テクスチャハンドル
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/uvChecker.png");
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	ImGui::Begin("SphericalCoordinate");
	ImGui::DragFloat("theta", &theta_, 0.01f);
	ImGui::DragFloat("phi", &phi_, 0.01f);
	ImGui::End();

	worldTransform_->translation_ = SphericalCoordinate(5.0f, theta_, phi_);

	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f }, true);
}