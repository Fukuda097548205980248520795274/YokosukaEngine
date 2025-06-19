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

	// 平行光源
	directionalLight0_ = std::make_unique<DirectionalLight>();
	directionalLight0_->Initialize();

	directionalLight1_ = std::make_unique<DirectionalLight>();
	directionalLight1_->Initialize();
	directionalLight1_->color_ = { 0.0f , 0.0f , 1.0f , 1.0f };
	directionalLight1_->direction_ = Normalize(Vector3(0.0f, 1.0f, 0.0f));

	// ポイントライト
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->position_ = { -5.0f ,  1.0f , 0.0f };
	pointLight_->intensity_ = 0.0f;

	// スポットライト
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->position_ = { 5.0f , 5.0f , 0.0f };
	spotLight_->intensity_ = 0.0f;
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

	engine_->SetDirectionalLight(directionalLight0_.get());
	engine_->SetDirectionalLight(directionalLight1_.get());

	// 描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f },
		pointLight_.get(), spotLight_.get());
}