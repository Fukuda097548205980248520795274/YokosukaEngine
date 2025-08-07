#include "EnemyDevilModelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void EnemyDevilModelHandles::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/body", "body.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/head", "head.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/armR", "armR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/armL", "armL.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/legR", "legR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/legL", "legL.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/wingR", "wingR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/wingL", "wingL.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyDevil/weapon", "weapon.obj"));
}