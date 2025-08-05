#include "ClearScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void ClearScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// 基底クラスの初期化
	BaseScene::Initialize(engine, modelHandleStore , textureHandleStore);
}

/// <summary>
/// 更新処理
/// </summary>
void ClearScene::Update()
{
	// 基底クラスの更新処理
	BaseScene::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void ClearScene::Draw()
{
	// 基底クラスの描画処理
	BaseScene::Draw();
}