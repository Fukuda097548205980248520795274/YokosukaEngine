#include "EnemyStingrayModelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void EnemyStingrayModelHandles::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyStingray/body", "body.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyStingray/wingR", "wingR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyStingray/wingL", "wingL.obj"));
}