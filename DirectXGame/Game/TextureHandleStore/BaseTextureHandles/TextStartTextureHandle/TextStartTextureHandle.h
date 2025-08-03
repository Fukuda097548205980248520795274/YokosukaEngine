#pragma once
#include "../BaseTextureHandles.h"

class TextStartTextureHandle : public BaseTextureHandles
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine) override;
};

