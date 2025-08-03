#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class BaseTextureHandles
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	virtual void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// テクスチャハンドルのGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetTextureHandles() { return textureHandle_; }


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// テクスチャハンドルの可変長配列
	uint32_t textureHandle_;
};

