#include "StageObjectSaturnPlanetModelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void StageObjectSaturnPlanetModelHandles::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/stageObject/stage2/planet", "planet.obj"));
}