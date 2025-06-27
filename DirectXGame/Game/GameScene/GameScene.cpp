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
	worldTransform_->scale_ = { 32.0f , 32.0f , 1.0f };

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/uvChecker.png");
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	ImGui::Begin("WorldTransform");
	ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
	ImGui::End();

	// トランスフォームの生成と初期化
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();

	Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);

	Vector3 pos = Transform(Vector3(0.0f , 0.0f , 0.0f), worldTransform_->worldMatrix_ * camera2d_->viewMatrix_ * camera2d_->projectionMatrix_);
	pos = Transform(pos, viewportMatrix);
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_.get(), textureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}