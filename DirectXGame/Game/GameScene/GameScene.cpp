#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	Scene::Initialize(engine);

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/uvChecker.png");
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// 基底クラスの更新処理
	Scene::Update();

	ImGui::Begin("worldTransform");
	ImGui::DragFloat3("rotation", &worldTransform_->rotation_.x, 0.01f);
	ImGui::End();

	// ワールド行列を取得する
	worldTransform_->worldMatrix_ =
		MakeScaleMatrix(worldTransform_->scale_) * 
		MakeRotateAxisAngle({ 1.0f ,0.0f , 0.0f }, worldTransform_->rotation_.x) *
		MakeRotateAxisAngle({ 0.0f ,1.0f , 0.0f }, worldTransform_->rotation_.y) *
		MakeRotateAxisAngle({ 0.0f ,0.0f , 1.0f }, worldTransform_->rotation_.z) *
		MakeTranslateMatrix(worldTransform_->translation_);

	// UVトランスフォームの更新
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// 平行光源の設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// 球を描画する
	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), textureHandle_, 12, 6, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);

	// 基底クラスの描画処理
	Scene::Draw();
}