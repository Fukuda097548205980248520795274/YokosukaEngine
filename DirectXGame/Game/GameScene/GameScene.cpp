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


	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();


	// ワールドトランスフォーム
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// UVトランスフォーム
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
	// Scene更新
	Scene::Update();


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

	engine_->SetDirectionalLight(directionalLight_.get());

	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), textureHandle_,16,16, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}