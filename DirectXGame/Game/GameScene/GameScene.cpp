#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// Scene更新
	Scene::Initialize(engine);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();
}