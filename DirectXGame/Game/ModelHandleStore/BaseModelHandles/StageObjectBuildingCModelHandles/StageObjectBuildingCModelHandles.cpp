#include "StageObjectBuildingCModelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void StageObjectBuildingCModelHandles::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/stageObject/building/building_C", "building_2.obj"));
}