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

	// 平行光源
	directionalLight0_ = std::make_unique<DirectionalLight>();
	directionalLight0_->Initialize();

	directionalLight1_ = std::make_unique<DirectionalLight>();
	directionalLight1_->Initialize();
	directionalLight1_->direction_ = Normalize(Vector3{ 0.0f , 1.0f , 0.0f });
	directionalLight1_->color_ = { 1.0f , 0.0f , 0.0f , 1.0f };

	directionalLight2_ = std::make_unique<DirectionalLight>();
	directionalLight2_->Initialize();
	directionalLight2_->direction_ = Normalize(Vector3{ 1.0f , 0.0f , 0.0f });
	directionalLight2_->color_ = { 0.0f , 0.0f , 1.0f , 1.0f };

	directionalLight3_ = std::make_unique<DirectionalLight>();
	directionalLight3_->Initialize();
	directionalLight3_->direction_ = Normalize(Vector3{ -1.0f , 0.0f , 0.0f });
	directionalLight3_->color_ = { 0.0f , 1.0f , 0.0f , 1.0f };



	// モデルハンドル
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");

	// ポイントライト
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライト
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;
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

	/*

	// 平行光源設置
	engine_->SetDirectionalLight(directionalLight0_.get());
	engine_->SetDirectionalLight(directionalLight1_.get());
	engine_->SetDirectionalLight(directionalLight2_.get());
	engine_->SetDirectionalLight(directionalLight3_.get());

	*/

	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f },
		pointLight_.get(), spotLight_.get());
}