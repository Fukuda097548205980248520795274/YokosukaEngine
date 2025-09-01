#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "BaseTextureHandles/WhiteTextureHandle/WhiteTextureHandle.h"

class TextureHandleStore
{
public:

	// ハンドルネーム
	enum HandleName
	{
		kWhite,

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

