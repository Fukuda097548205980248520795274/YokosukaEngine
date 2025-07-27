#include "EnemyButterflyodelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void EnemyButterflyodelHandles::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/body", "body.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/wingR", "wingR.obj"));
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/wingL", "wingL.obj"));
}