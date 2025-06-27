#include "GravitationalField.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void GravitationalField::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, float radius)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	radius_ = radius;


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ *= radius_;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/gravitationalField", "gravitationalField.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void GravitationalField::Update()
{
	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void GravitationalField::Draw()
{
	// 球を描画する
	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 0.1f), false);
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 GravitationalField::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;
	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];
	return worldPosition;
}

/// <summary>
/// 当たり判定の球のGetter
/// </summary>
/// <returns></returns>
Sphere GravitationalField::GetCollisionSphere() const
{
	Sphere sphere;
	sphere.center = GetWorldPosition();
	sphere.radius = radius_;
	return sphere;
}

/// <summary>
/// 衝突判定応答
/// </summary>
void GravitationalField::OnCollision()
{
	
}