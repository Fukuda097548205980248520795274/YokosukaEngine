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

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 平面の生成と初期化
	plane_ = std::make_unique<Plane>();
	plane_->Initialize(engine_ , camera3d_.get());

	// スプライトの生成と初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(engine_ , camera2d_.get());

	// 球の生成と初期化
	sphere_ = std::make_unique<SphereModel>();
	sphere_->Initialize(engine_, camera3d_.get());

	// スザンヌの生成と初期化
	suzanne_ = std::make_unique<Suzanne>();
	suzanne_->Initialize(engine_, camera3d_.get());

	// スタンフォードバニーの生成と初期化
	stanfordBunny_ = std::make_unique<StanfordBunny>();
	stanfordBunny_->Initialize(engine_ , camera3d_.get());

	// ユタティーポットの生成と初期化
	utahTeapot_ = std::make_unique<UtahTeapot>();
	utahTeapot_->Initialize(engine_, camera3d_.get());

	// マルチメッシュの生成と初期化
	multiMesh_ = std::make_unique<MultiMesh>();
	multiMesh_->Initialize(engine_, camera3d_.get());

	// マルチマテリアルの生成と初期化
	multiMaterial_ = std::make_unique<MultiMaterial>();
	multiMaterial_->Initialize(engine_, camera3d_.get());
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

	// スザンヌの更新
	suzanne_->Update();

	// スタンフォーダニーの更新
	stanfordBunny_->Update();

	// ユタティーポットの更新
	utahTeapot_->Update();

	// マルチメッシュの更新
	multiMesh_->Update();

	// マルチマテリアルの更新
	multiMaterial_->Update();

	ImGui::End();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// 平行光源を設置する
	engine_->SetDirectionalLight(directionalLight_.get());

	// 平面の描画
	plane_->Draw();

	// スプライトの描画
	sprite_->Draw();

	// 球の描画
	sphere_->Draw();

	// スザンヌの描画
	suzanne_->Draw();

	// スタンフォーダニーの描画
	stanfordBunny_->Draw();

	// ユタティーポットの描画
	utahTeapot_->Draw();

	// マルチメッシュの描画
	multiMesh_->Draw();

	// マルチマテリアルの描画
	multiMaterial_->Draw();
}