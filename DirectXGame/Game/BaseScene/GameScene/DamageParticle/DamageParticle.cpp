#include "DamageParticle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void DamageParticle::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d , const Vector3& position)
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
	worldTransform_->translation_ = position;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void DamageParticle::Update()
{
	// タイマーを進める
	timer_ += 1.0f / 60.0f;

	if (timer_ >= maxTime_)
	{
		isFinished_ = true;
		return;
	}

	Matrix4x4 backToFrontMatrix = MakeRotateXMatrix(std::numbers::pi_v<float> / 2.0f);
	Matrix4x4 billboardMatrix = backToFrontMatrix * MakeAffineMatrix(camera3d_->scale_, camera3d_->rotation_, camera3d_->translation_);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// トランスフォームの更新
	worldTransform_->worldMatrix_ = MakeScaleMatrix(worldTransform_->scale_) * billboardMatrix * MakeTranslateMatrix(worldTransform_->translation_);
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void DamageParticle::Draw()
{
	engine_->DrawRing(worldTransform_.get(), uvTransform_.get(), camera3d_, textureHandle_, 12, 12.0f, 8.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
}