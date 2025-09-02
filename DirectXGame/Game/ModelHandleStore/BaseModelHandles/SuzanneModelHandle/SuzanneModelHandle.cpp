#include "SuzanneModelHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void SuzanneModelHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");
}