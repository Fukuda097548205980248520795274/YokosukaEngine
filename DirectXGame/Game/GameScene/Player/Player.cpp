#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(directionalLight);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	directionalLight_ = directionalLight;


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Player", "Player.obj");

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 0.0f, 0.0f, 0.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());
}