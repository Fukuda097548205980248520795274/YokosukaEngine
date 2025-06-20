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
	directionalLight0_->intensity_ = 0.3f;

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
	modelHandle_ = engine_->LoadModelData("./Resources/Models/terrain", "terrain.obj");

	// ポイントライト
	pointLight0_ = std::make_unique<PointLight>();
	pointLight0_->Initialize();
	pointLight0_->position_ = { -2.0f , 1.5f , 0.0f };
	pointLight0_->color_ = { 1.0f , 1.0f , 1.0f , 1.0f };
	pointLight0_->intensity_ = 62.0f;
	pointLight0_->radius_ = 6.0f;
	pointLight0_->decay_ = 6.0f;

	pointLight1_ = std::make_unique<PointLight>();
	pointLight1_->Initialize();
	pointLight1_->position_ = { 0.0f , 1.5f , 2.0f };
	pointLight1_->color_ = { 1.0f , 0.0f , 0.0f , 1.0f };
	pointLight1_->intensity_ = 62.0f;
	pointLight1_->radius_ = 6.0f;
	pointLight1_->decay_ = 6.0f;

	pointLight2_ = std::make_unique<PointLight>();
	pointLight2_->Initialize();
	pointLight2_->position_ = { 0.0f , 1.5f , -2.0f };
	pointLight2_->color_ = { 0.0f , 1.0f , 0.0f , 1.0f };
	pointLight2_->intensity_ = 62.0f;
	pointLight2_->radius_ = 6.0f;
	pointLight2_->decay_ = 6.0f;

	pointLight3_ = std::make_unique<PointLight>();
	pointLight3_->Initialize();
	pointLight3_->position_ = { 0.0f , 2.0f , 2.0f };
	pointLight3_->color_ = { 0.0f , 0.0f , 1.0f , 1.0f };
	pointLight3_->intensity_ = 62.0f;
	pointLight3_->decay_ = 6.0f;

	// スポットライト
	spotLight0_ = std::make_unique<SpotLight>();
	spotLight0_->Initialize();
	spotLight0_->position_ = { -6.0f , 4.0f , 0.0f };
	spotLight0_->color_ = { 1.0f , 1.0f , 1.0f , 1.0f };
	spotLight0_->decay_ = 1.0f;

	spotLight1_ = std::make_unique<SpotLight>();
	spotLight1_->Initialize();
	spotLight1_->position_ = { 6.0f , 4.0f , 0.0f };
	spotLight1_->color_ = { 1.0f , 0.0f , 0.0f , 1.0f };
	spotLight1_->decay_ = 1.0f;

	spotLight2_ = std::make_unique<SpotLight>();
	spotLight2_->Initialize();
	spotLight2_->position_ = { 0.0f , 4.0f , 6.0f };
	spotLight2_->color_ = { 0.0f , 1.0f , 0.0f , 1.0f };
	spotLight2_->decay_ = 1.0f;

	spotLight3_ = std::make_unique<SpotLight>();
	spotLight3_->Initialize();
	spotLight3_->position_ = { 0.0f , 4.0f , -6.0f };
	spotLight3_->color_ = { 0.0f , 0.0f , 1.0f , 1.0f };
	spotLight3_->decay_ = 1.0f;
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

	ImGui::Begin("pointLight0");
	ImGui::DragFloat3("position", &pointLight0_->position_.x, 0.1f);
	ImGui::End();

	ImGui::Begin("pointLigh1");
	ImGui::DragFloat3("position", &pointLight1_->position_.x, 0.1f);
	ImGui::End();

	ImGui::Begin("pointLigh2");
	ImGui::DragFloat3("position", &pointLight2_->position_.x, 0.1f);
	ImGui::End();

	// ポイントライト
	/*
	engine_->SetPointLight(pointLight2_.get());
	engine_->SetPointLight(pointLight1_.get());
	engine_->SetPointLight(pointLight3_.get());
	engine_->SetPointLight(pointLight0_.get());
	*/

	// スポットライトを設置する
	engine_->SetSpotLight(spotLight0_.get());
	engine_->SetSpotLight(spotLight1_.get());
	engine_->SetSpotLight(spotLight2_.get());
	engine_->SetSpotLight(spotLight3_.get());

	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f });
}