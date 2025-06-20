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
	modelHandle_ = engine_->LoadModelData("./Resources/Models/plane", "plane.obj");

	// 赤いポイントライト
	redPointLight_ = std::make_unique<PointLight>();
	redPointLight_->Initialize();
	redPointLight_->color_ = { 1.0f , 0.0f , 0.0f , 1.0f };
	redPointLight_->position_ = { -0.5f , 0.5f , 0.0f };

	// 青いポイントライト
	bluePointLight_ = std::make_unique<PointLight>();
	bluePointLight_->Initialize();
	bluePointLight_->color_ = { 0.0f , 0.0f , 1.0f , 1.0f };
	bluePointLight_->position_ = { 0.5f , 0.5f , 0.0f };
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

	// ライト設置
	engine_->SetPointLight(redPointLight_.get());
	engine_->SetPointLight(bluePointLight_.get());

	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f }, true);
}