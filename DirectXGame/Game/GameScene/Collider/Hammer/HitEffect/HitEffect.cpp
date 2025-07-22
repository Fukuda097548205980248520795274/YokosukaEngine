#include "HitEffect.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void HitEffect::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
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
	worldTransform_->scale_ *= 0.1f;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャハンドル
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void HitEffect::Update()
{
	worldTransform_->scale_ += {0.1f, 0.1f, 0.1f};

	// タイマーを進める
	finishedTimer_ += 1.0f / 60.0f;
	if (finishedTimer_ >= kFinishedTime)
	{
		isFinished_ = true;
	}

	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void HitEffect::Draw()
{
	// 球を描画する
	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_, textureHandle_, 16, 8, Vector4(0.5f, 0.5f, 1.0f, 0.1f), false);
}