#include "CenterAxis.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void CenterAxis::Initliaze(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受けとる
	engine_ = engine;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_.y = 10.0f;

	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void CenterAxis::Update()
{
	worldTransform_->translation_.z += 1.0f;
	worldTransform_->translation_.z = std::fmod(worldTransform_->translation_.z, 720.0f);

	// ワールドトラスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void CenterAxis::Draw()
{
	// 弾を描画する
	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_, textureHandle_, 10, 5, Vector4(1.0f, 0.0f, 0.0f, 1.0f), false);
}


/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 CenterAxis::GetWorldPosition()const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}