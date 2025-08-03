#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "BaseTextureHandles/TextStartTextureHandle/TextStartTextureHandle.h"
#include "BaseTextureHandles/TextAButtonTextureHandle/TextAButtonTextureHandle.h"
#include "BaseTextureHandles/TextLStickTextureHandle/TextLStickTextureHandle.h"
#include "BaseTextureHandles/TextLStickThumbTextureHandle/TextLStrickThumbTextureHandle.h"

class TextureHandleStore
{
public:

	// ハンドルネーム
	enum HandleName
	{
		kStart,
		kAButton,
		kLStick,
		kLStickThumb,
		kNumHandleName
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// テクスチャハンドルGetter
	/// </summary>
	/// <param name="handleName"></param>
	/// <returns></returns>
	uint32_t GetTextureHandle(HandleName handleName) const { return textureHandles_[handleName]->GetTextureHandles(); }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// テクスチャハンドル
	std::unique_ptr<BaseTextureHandles> textureHandles_[kNumHandleName];
};

