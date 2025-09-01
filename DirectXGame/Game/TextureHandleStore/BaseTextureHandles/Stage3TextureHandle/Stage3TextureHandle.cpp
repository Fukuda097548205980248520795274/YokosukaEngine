#include "Stage3TextureHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void Stage3TextureHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseTextureHandles::Initialize(engine);

	// モデルを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/stageSelect/stage3.png");
}