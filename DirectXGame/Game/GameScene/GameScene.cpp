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

	// uvトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	ImGui::Begin("slerp");
	ImGui::SliderFloat("t", &t, 0.0f, 1.0f);
	ImGui::End();

	worldTransform_->translation_ = Slerp(start, end, t);

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

	// 平行光源を設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f }, true);
}