#include "Stage1TextureHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void Stage1TextureHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseTextureHandles::Initialize(engine);

	// モデルを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/stageSelect/stage1.png");
}