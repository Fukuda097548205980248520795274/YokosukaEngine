#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "BaseTextureHandles/WhiteTextureHandle/WhiteTextureHandle.h"
#include "BaseTextureHandles/ReticleTextureHandle/ReticleTextureHandle.h"

#include "BaseTextureHandles/TextGamepadStartTextureHandle/TextGamepadStartTextureHandle.h"
#include "BaseTextureHandles/TextGamepadMoveTextureHandle/TextGamepadMoveTextureHandle.h"
#include "BaseTextureHandles/TextGamepadShotTextureHandle/TextGamepadShotTextureHandle.h"
#include "BaseTextureHandles/TextGamepadTimeStopTextureHandle/TextGamepadTimeStopTextureHandle.h"
#include "BaseTextureHandles/TextGamepadPoseTextureHandle/TextGamepadPoseTextureHandle.h"

#include "BaseTextureHandles/TextkeyboardStartTextureHandle/TextkeyboardStartTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardMoveTextureHandle/TextKeyboardMoveTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardShotTextureHandle/TextKeyboardShotTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardTimeStopTextureHandle/TextKeyboardTimeStopTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardPoseTextureHandle/TextKeyboardPoseTextureHandle.h"

#include "BaseTextureHandles/TextStageServiceRailwayTextureHandle/TextStageServiceRailwayTextureHandle.h"
#include "BaseTextureHandles/TextStageEfforTownTextureHandle/TextStageEfforTownTextureHandle.h"
#include "BaseTextureHandles/TextStageTechnoRootTextureHandle/TextStageTechnoRootTextureHandle.h"
#include "BaseTextureHandles/TextStageImaginaryPlamTextureHandle/TextStageImaginaryPlamTextureHandle.h"

class TextureHandleStore
{
public:

	// ハンドルネーム
	enum HandleName
	{
		kWhite,
		kReticle,

		kGamepadStart,
		kGamepadMove,
		kGamepadShot,
		kGamepadTimeStop,
		kGamepadPose,

		kKeyboardStart,
		kKeyboardMove,
		kKeyboardShot,
		kKeyboardTimeStop,
		kKeyboardPose,

		kStageServiceRailway,
		kStageEfforTown,
		kStageTechnoRoot,
		kStageImaginaryPlam,

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

