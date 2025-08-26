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


	// ゲームパッド : スタート の生成と初期化
	spriteGamepadStart_ = std::make_unique<Sprite>();
	spriteGamepadStart_->Initialize(engine_, camera2d_, textureHandleStore_->GetTextureHandle(TextureHandleStore::kGamepadStart));
	spriteGamepadStart_->worldTransform_->scale_ =
		Vector3(static_cast<float>(engine_->GetTextureWidth(textureHandleStore_->GetTextureHandle(TextureHandleStore::kGamepadStart))) / 2.0f,
			static_cast<float>(engine_->GetTextureHeight(textureHandleStore_->GetTextureHandle(TextureHandleStore::kGamepadStart))) / 2.0f, 0.0f);
	spriteGamepadStart_->worldTransform_->translation_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, 500.0f, 0.0f);

	// キーボード : スタート の生成と初期化
	spritekeyboardStart_ = std::make_unique<Sprite>();
	spritekeyboardStart_->Initialize(engine_, camera2d_, textureHandleStore_->GetTextureHandle(TextureHandleStore::kKeyboardStart));
	spritekeyboardStart_->worldTransform_->scale_ =
		Vector3(static_cast<float>(engine_->GetTextureWidth(textureHandleStore_->GetTextureHandle(TextureHandleStore::kKeyboardStart))) / 2.0f,
			static_cast<float>(engine_->GetTextureHeight(textureHandleStore_->GetTextureHandle(TextureHandleStore::kKeyboardStart))) / 2.0f, 0.0f);
	spritekeyboardStart_->worldTransform_->translation_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, 500.0f, 0.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void TitleHud::Update()
{
	// スプライトの更新
	spriteGamepadStart_->Update();
	spritekeyboardStart_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void TitleHud::Draw()
{
	// コントローラを使用しているかどうか
	if (engine_->IsGamepadEnable(0))
	{
		spriteGamepadStart_->Draw();
	}
	else
	{
		spritekeyboardStart_->Draw();
	}
}