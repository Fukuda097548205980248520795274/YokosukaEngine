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
	worldTransform_->scale_;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/uvChecker.png");


	// ポイントライトの生成と初期化
	pointLight0_ = std::make_unique<PointLight>();
	pointLight0_->Initialize();
	pointLight0_->color_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	pointLight0_->position_ = Vector3(-1.0f, 2.0f, 0.0f);

	pointLight1_ = std::make_unique<PointLight>();
	pointLight1_->Initialize();
	pointLight1_->color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	pointLight1_->position_ = Vector3(1.0f, 2.0f, 0.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	ImGui::Begin("Plane");
	ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
	ImGui::DragFloat3("rotation", &worldTransform_->rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &worldTransform_->scale_.x, 0.01f);
	ImGui::End();

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

	// ポイントライトを設置する
	engine_->SetPointLight(pointLight0_.get());
	engine_->SetPointLight(pointLight1_.get());

	// 球を描画する
	engine_->DrawCylinder(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), textureHandle_, 32, 5.0f, 3.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}