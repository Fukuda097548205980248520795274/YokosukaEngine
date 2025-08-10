#include "PlayerHUDTextHpModelHandle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void PlayerHUDTextHpModelHandle::Initialize(const YokosukaEngine* engine)
{
	// 基底クラスの初期化
	BaseModelHandles::Initialize(engine);

	// モデルを読み込む
	modelHandles_.push_back(engine_->LoadModelData("./Resources/Models/Player/Hud/textHp", "textHp.obj"));
}