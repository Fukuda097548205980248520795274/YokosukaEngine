#include "ModelHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void ModelHandleStore::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	
	// モデル : スザンヌ
	modelHandles_[kSuzanne] = std::make_unique<SuzanneModelHandle>();
	modelHandles_[kSuzanne]->Initialize(engine_);
}