#include "UniverseModelHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void UniverseModelHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/stageSelect/universe", "universe.obj"));
}