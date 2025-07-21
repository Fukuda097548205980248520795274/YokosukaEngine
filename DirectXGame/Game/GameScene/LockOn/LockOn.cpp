#include "LockOn.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
void LockOn::Initialize(const YokosukaEngine* engine, const Camera2D* camera2d)
{
	// nullptrチェック
	assert(engine);
	assert(camera2d);

	// 引数を受け取る
	engine_ = engine;
	camera2d_ = camera2d;
}

/// <summary>
/// 更新処理
/// </summary>
void LockOn::Update()
{

}

/// <summary>
/// 描画処理
/// </summary>
void LockOn::Draw()
{

}