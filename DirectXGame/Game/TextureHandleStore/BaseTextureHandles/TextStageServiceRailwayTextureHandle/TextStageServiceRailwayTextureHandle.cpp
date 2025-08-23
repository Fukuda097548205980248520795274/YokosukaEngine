#include "TextStageServiceRailwayTextureHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void TextStageServiceRailwayTextureHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseTextureHandles::Initialize(engine);

	// モデルを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/texts/serviceRailway.png");
}