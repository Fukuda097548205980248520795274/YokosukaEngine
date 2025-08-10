#include "PoseScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="camera2d"></param>
void PoseScene::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(camera2d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	camera2d_ = camera2d;
}

/// <summary>
/// 更新処理
/// </summary>
void PoseScene::Update()
{

}

/// <summary>
/// 描画処理
/// </summary>
void PoseScene::Draw()
{

}