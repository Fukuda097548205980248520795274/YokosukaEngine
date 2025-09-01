#include "GameScene.h"
#include "../../ModelHandleStore/ModelHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// BaseScene更新
	BaseScene::Initialize(engine , modelHandleStore , textureHandleStore);

	// スプライトの生成と初期化
	spriteWhite_ = std::make_unique<Sprite>();
	spriteWhite_->Initialize(engine_  , camera2d_.get() , Vector2(0.0f , 0.0f) , textureHandleStore_->GetTextureHandle(TextureHandleStore::kUvChecker));
	spriteWhite_->worldTransform_->translation_ = Vector3(12.0f, 12.0f, 0.0f);
	spriteWhite_->worldTransform_->scale_ = Vector3(300.0f, 300.0f , 0.0f);
	
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// スプライトの更新
	spriteWhite_->Update();

	// Scene更新
	BaseScene::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// スプライトの描画
	spriteWhite_->Draw();

	// Scene描画
	BaseScene::Draw();
}