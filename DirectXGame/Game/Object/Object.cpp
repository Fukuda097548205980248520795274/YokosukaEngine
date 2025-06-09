#include "Object.h"

/// <summary>
/// 初期化
/// </summary>
void Object::Initialize(const YokosukaEngine* engine , const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->rotation_.y = float(std::numbers::pi);
	worldTransform_->translation_.z = 10.0f;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.gltf");

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLight_->intensity_ = 1.0f;

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->position_ = { -1.0f , 1.0f , -1.0f };
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->position_ = { 0.0f , 5.0f , 0.0f };
	spotLight_->intensity_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void Object::Update()
{
	// ←キーで左移動
	if (engine_->GetKeyPress(DIK_LEFT))
	{
		worldTransform_->translation_.x -= 0.1f;
		uvTransform_->translation_.x += 0.01f;
	}

	// →キーで右移動
	if (engine_->GetKeyPress(DIK_RIGHT))
	{
		worldTransform_->translation_.x += 0.1f;
		uvTransform_->translation_.x -= 0.01f;
	}

	// ↑キーで上移動
	if (engine_->GetKeyPress(DIK_UP))
	{
		worldTransform_->translation_.y += 0.1f;
		uvTransform_->translation_.y += 0.01f;
	}

	// ↓キーで下移動
	if (engine_->GetKeyPress(DIK_DOWN))
	{
		worldTransform_->translation_.y -= 0.1f;
		uvTransform_->translation_.y -= 0.01f;
	}

	// トランスフォーム更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Object::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 1.0f , 1.0f ,1.0f ,1.0f },
		directionalLight_.get(), pointLight_.get(), spotLight_.get());
}