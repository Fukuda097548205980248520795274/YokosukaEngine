#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class BaseModelHandles
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	virtual void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// モデルハンドルのGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetModelHandles() { return modelHandles_; }


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// モデルハンドルの可変長配列
	uint32_t modelHandles_;
};

