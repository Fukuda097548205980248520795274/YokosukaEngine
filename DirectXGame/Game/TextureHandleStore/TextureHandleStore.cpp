#include "TextureHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void TextureHandleStore::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;


	// スタートのテクスチャ
	textureHandles_[kStart] = std::make_unique<TextStartTextureHandle>();
	textureHandles_[kStart]->Initialize(engine_);

	// Aボタンのテクスチャ
	textureHandles_[kAButton] = std::make_unique<TextAButtonTextureHandle>();
	textureHandles_[kAButton]->Initialize(engine_);

	// Lスティックのテクスチャ
	textureHandles_[kLStick] = std::make_unique<TextLStickTextureHandle>();
	textureHandles_[kLStick]->Initialize(engine_);

	// Lスティック押し込みのテクスチャ
	textureHandles_[kLStickThumb] = std::make_unique<TextLStrickThumbTextureHandle>();
	textureHandles_[kLStickThumb]->Initialize(engine_);
}