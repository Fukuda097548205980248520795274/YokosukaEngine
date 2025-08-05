#include "EnemyBirdModelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void EnemyBirdModelHandles::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyBird/body", "body.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyBird/topWingR", "topWingR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyBird/topWingL", "topWingL.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyBird/bottomWingR", "bottomWingR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyBird/bottomWingL", "bottomWingL.obj"));
}