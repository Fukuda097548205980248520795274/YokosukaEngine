#include "BaseSprite.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
/// <param name="textureHandle"></param>
void Sprite::Initialize(const YokosukaEngine* engine, const Camera2D* camera2d, uint32_t textureHandle)
{
	// nullptrチェック
	assert(engine);
	assert(camera2d);

	// 引数を受け取る
	engine_ = engine;
	camera2d_ = camera2d;
	textureHandle_ = textureHandle;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform2D>();
	worldTransform_->Initialize();
	worldTransform_->scale_ =
		Vector3(static_cast<float>(engine_->GetTextureWidth(textureHandle_)) / 2.0f, static_cast<float>(engine_->GetTextureHeight(textureHandle_)) / 2.0f, 0.0f);

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void Sprite::Update()
{
	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Sprite::Draw()
{
	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_, textureHandle_, color_, FillMode::kSolid);
}