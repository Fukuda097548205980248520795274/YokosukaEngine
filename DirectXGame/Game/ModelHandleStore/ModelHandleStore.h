#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "BaseModelHandles/BaseModelHandles.h"

class ModelHandleStore
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine);


	// ハンドルネーム
	enum HandleName
	{
		kNan,
		kNumHandleName,
	};

	/// <summary>
	/// モデルハンドルのGetter
	/// </summary>
	/// <param name="handleName"></param>
	/// <returns></returns>
	std::vector<uint32_t> GetModelHandle(HandleName handleName) const { return modelHandles_[handleName]->GetModelHandles(); }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// モデルハンドル
	std::unique_ptr<BaseModelHandles> modelHandles_[kNumHandleName];
};

