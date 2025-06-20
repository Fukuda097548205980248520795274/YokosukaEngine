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

	// ワールドトランスフォーム
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルハンドル
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");

	// ポイントライト
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();

	// スポットライト
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	// トランスフォームを更新
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

	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f },
		pointLight_.get(), spotLight_.get());
}