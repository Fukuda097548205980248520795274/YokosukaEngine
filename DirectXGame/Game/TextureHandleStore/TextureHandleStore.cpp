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


	// 画像 : 白
	textureHandles_[kWhite] = std::make_unique<WhiteTextureHandle>();
	textureHandles_[kWhite]->Initialize(engine_);
}