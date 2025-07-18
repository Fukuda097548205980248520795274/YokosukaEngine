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

	// 平面の生成と初期化
	plane_ = std::make_unique<Plane>();
	plane_->Initialize(engine_ , camera3d_.get());

	// スプライトの生成と初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(engine_ , camera2d_.get());

	// 球の生成と初期化
	sphere_ = std::make_unique<SphereModel>();
	sphere_->Initialize(engine_, camera3d_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	ImGui::Begin("CG2");

	// 平面の更新
	plane_->Update();

	// スプライトの更新
	sprite_->Update();

	// 球の更新
	sphere_->Update();

	ImGui::End();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// 平面の描画
	plane_->Draw();

	// スプライトの描画
	sprite_->Draw();

	// 球の描画
	sphere_->Draw();
}