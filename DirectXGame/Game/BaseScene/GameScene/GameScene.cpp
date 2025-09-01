#include "GameScene.h"
#include "../../ModelHandleStore/ModelHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// BaseScene更新
	BaseScene::Initialize(engine , modelHandleStore , textureHandleStore);

	
	
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	BaseScene::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	BaseScene::Draw();
}