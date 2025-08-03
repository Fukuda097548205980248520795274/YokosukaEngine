#include "PlayerStateHud.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="camera2d"></param>
void PlayerStateHud::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d,
	const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(camera2d);
	assert(modelHandleStore);
	assert(textureHandleStore);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	camera2d_ = camera2d;
	modelHandleStore_ = modelHandleStore;
	textureHandleStore_ = textureHandleStore;


	// Aボタン
	aButtonWorldTransform_ = std::make_unique<WorldTransform2D>();
	aButtonWorldTransform_->Initialize();
	aButtonWorldTransform_->scale_ = Vector3(96.0f, 32.0f, 0.0f);
	aButtonWorldTransform_->translation_ = Vector3(96.0f, 32.0f, 0.0f);
	aButtonUvTransform_ = std::make_unique<UvTransform>();
	aButtonUvTransform_->Initialize();
	aButtonTextureHandle_ = textureHandleStore_->GetTextureHandle(TextureHandleStore::kAButton);


	// Aボタン
	lStickWorldTransform_ = std::make_unique<WorldTransform2D>();
	lStickWorldTransform_->Initialize();
	lStickWorldTransform_->scale_ = Vector3(96.0f, 32.0f, 0.0f);
	lStickWorldTransform_->translation_ = Vector3(96.0f, 152.0f, 0.0f);
	lStickUvTransform_ = std::make_unique<UvTransform>();
	lStickUvTransform_->Initialize();
	lStickTextureHandle_ = textureHandleStore_->GetTextureHandle(TextureHandleStore::kLStick);


	// Aボタン
	lStickThumbWorldTransform_ = std::make_unique<WorldTransform2D>();
	lStickThumbWorldTransform_->Initialize();
	lStickThumbWorldTransform_->scale_ = Vector3(96.0f, 32.0f, 0.0f);
	lStickThumbWorldTransform_->translation_ = Vector3(96.0f, 272.0f, 0.0f);
	lStickThumbUvTransform_ = std::make_unique<UvTransform>();
	lStickThumbUvTransform_->Initialize();
	lStickThumbTextureHandle_ = textureHandleStore_->GetTextureHandle(TextureHandleStore::kLStickThumb);
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerStateHud::Update()
{
	aButtonWorldTransform_->UpdateWorldMatrix();
	aButtonUvTransform_->UpdateWorldMatrix();

	lStickWorldTransform_->UpdateWorldMatrix();
	lStickUvTransform_->UpdateWorldMatrix();

	lStickThumbWorldTransform_->UpdateWorldMatrix();
	lStickThumbUvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerStateHud::Draw()
{
	// スプライトを描画する
	engine_->DrawSprite(aButtonWorldTransform_.get(), aButtonUvTransform_.get(), camera2d_, aButtonTextureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	engine_->DrawSprite(lStickWorldTransform_.get(), lStickUvTransform_.get(), camera2d_, lStickTextureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	engine_->DrawSprite(lStickThumbWorldTransform_.get(), lStickThumbUvTransform_.get(), camera2d_, lStickThumbTextureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}