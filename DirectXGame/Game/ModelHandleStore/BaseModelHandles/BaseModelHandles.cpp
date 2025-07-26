#include "BaseModelHandles.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void BaseModelHandles::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;
}