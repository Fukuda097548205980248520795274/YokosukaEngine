#include "StageObjectHouseModelHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void StageObjectHouseModelHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/stageObject/stage3/house", "house.obj"));
}