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

	// ワールドトラスフォーム
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_.y = radius_;
	worldTransform_->translation_.z = radius_;

	// uvトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャハンドル
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();


	// 速度ベクトル
	Vector3 velocity = { 0.0f , 0.0f , 0.0f };

	// プレイヤーのベクトル
	Vector3 toPlayer = Normalize(worldTransform_->translation_);

	velocity = Vector3(-toPlayer.y, toPlayer.x , toPlayer.z);

	worldTransform_->translation_ += velocity;

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

	// 平行光源の設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// 球を描画する
	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), textureHandle_, 16, 16, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}