#include "PoseReturnGameTextureHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void PoseReturnGameTextureHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseTextureHandles::Initialize(engine);

	// モデルを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/Pose/returnGame.png");
}