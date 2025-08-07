#include "EnemyFairyModelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void EnemyFairyModelHandles::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/body", "body.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/head", "head.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/armR", "armR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/armL", "armL.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/legR", "legR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/legL", "legL.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/wingR", "wingR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/wingL", "wingL.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyFairy/weapon", "weapon.obj"));
}