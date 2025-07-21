#include "Collider.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Collider::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
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

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/shape/colliderSphere", "colliderSphere.obj");
}

/// <summary>
/// トランスフォームの更新処理
/// </summary>
void Collider::UpdateTransform()
{
	worldTransform_->translation_ = GetCenterPosition();

	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Collider::Draw()
{
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, Vector4(0.8f, 0.8f , 0.8f, 1.0f), false);
}