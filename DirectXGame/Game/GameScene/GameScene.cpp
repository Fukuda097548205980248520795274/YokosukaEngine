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

	controlPoints_ =
	{
		{0.0f , 0.0f , 0.0f},
		{10.0f , 10.0f , 0.0f},
		{10.0f , 15.0f , 0.0f},
		{20.0f , 15.0f , 0.0f},
		{20.0f , 0.0f , 0.0f},
		{30.0f , 5.0f , 0.0f}
	};
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

	engine_->DrwaCatmullRomSpline(controlPoints_, { 1.0f , 0.0f , 0.0f , 1.0f }, camera3d_.get());
}