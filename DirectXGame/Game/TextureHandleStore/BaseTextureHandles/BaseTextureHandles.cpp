#include "BaseTextureHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void BaseTextureHandles::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;
}