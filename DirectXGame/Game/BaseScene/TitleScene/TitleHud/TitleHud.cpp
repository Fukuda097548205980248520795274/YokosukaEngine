#include "TitleHud.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="camera2d"></param>
void TitleHud::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d,
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


	// スタート
	startWorldTransform_ = std::make_unique<WorldTransform2D>();
	startWorldTransform_->Initialize();
	startWorldTransform_->scale_ = Vector3(192.0f, 64.0f, 0.0f);
	startWorldTransform_->translation_ = Vector3(640.0f, 320.0f, 0.0f);
	startUvTransform_ = std::make_unique<UvTransform>();
	startUvTransform_->Initialize();
	startTextureHandle_ = textureHandleStore_->GetTextureHandle(TextureHandleStore::kStart);
}

/// <summary>
/// 更新処理
/// </summary>
void TitleHud::Update()
{
	startWorldTransform_->UpdateWorldMatrix();
	startUvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void TitleHud::Draw()
{
	// スプライトを描画する
	engine_->DrawSprite(startWorldTransform_.get(), startUvTransform_.get(), camera2d_, startTextureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}