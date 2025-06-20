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
	modelHandle_ = engine_->LoadModelData("./Resources/Models/terrain", "terrain.obj");

	// ポイントライト
	pointLight0_ = std::make_unique<PointLight>();
	pointLight0_->Initialize();
	pointLight0_->position_ = { -5.0f ,  2.0f , 0.0f };
	pointLight0_->radius_ = 6.0f;
	pointLight0_->intensity_ = 32.0f;

	pointLight1_ = std::make_unique<PointLight>();
	pointLight1_->Initialize();
	pointLight1_->position_ = { 5.0f ,  2.0f , 0.0f };
	pointLight1_->radius_ = 6.0f;
	pointLight1_->intensity_ = 32.0f;

	pointLight2_ = std::make_unique<PointLight>();
	pointLight2_->Initialize();
	pointLight2_->position_ = { 0.0f ,  2.0f , 5.0f };
	pointLight2_->radius_ = 6.0f;
	pointLight2_->intensity_ = 32.0f;

	pointLight3_ = std::make_unique<PointLight>();
	pointLight3_->Initialize();
	pointLight3_->position_ = { 0.0f ,  2.0f , -5.0f };
	pointLight3_->radius_ = 6.0f;
	pointLight3_->intensity_ = 32.0f;

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

	engine_->SetPointLight(pointLight0_.get());
	engine_->SetPointLight(pointLight1_.get());
	engine_->SetPointLight(pointLight2_.get());
	engine_->SetPointLight(pointLight3_.get());

	// 描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f },spotLight_.get());
}