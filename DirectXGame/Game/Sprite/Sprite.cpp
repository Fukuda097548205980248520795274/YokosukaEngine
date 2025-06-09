#include "Sprite.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
void Sprite::Initialize(const YokosukaEngine* engine, const Camera2D* camera2d)
{
	// nullptrチェック
	assert(engine);
	assert(camera2d);

	// 引数を受け取る
	engine_ = engine;
	camera2d_ = camera2d;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = { 500.0f , 270.0f };
	worldTransform_->translation_.x = worldTransform_->scale_.x;
	worldTransform_->translation_.y = 720.0f - worldTransform_->scale_.y;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/explanation.png");
}

/// <summary>
/// 更新処理
/// </summary>
void Sprite::Update()
{
	// トランスフォームを更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Sprite::Draw()
{
	// スプライトを描画する
	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_, textureHandle_, { 1.0f , 1.0f , 1.0f , 1.0f });
}