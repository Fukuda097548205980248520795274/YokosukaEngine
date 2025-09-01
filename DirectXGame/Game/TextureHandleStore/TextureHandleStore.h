#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

#include "BaseTextureHandles/WhiteTextureHandle/WhiteTextureHandle.h"
#include "BaseTextureHandles/ReticleTextureHandle/ReticleTextureHandle.h"

#include "BaseTextureHandles/TitleTextureHandle/TitleTextureHandle.h"

#include "BaseTextureHandles/Stage1TextureHandle/Stage1TextureHandle.h"
#include "BaseTextureHandles/Stage2TextureHandle/Stage2TextureHandle.h"
#include "BaseTextureHandles/Stage3TextureHandle/Stage3TextureHandle.h"

#include "BaseTextureHandles/TextGamepadStageSelectTextureHandle/TextGamepadStageSelectTextureHandle.h"
#include "BaseTextureHandles/TextGamepadStartTextureHandle/TextGamepadStartTextureHandle.h"
#include "BaseTextureHandles/TextGamepadMoveTextureHandle/TextGamepadMoveTextureHandle.h"
#include "BaseTextureHandles/TextGamepadShotTextureHandle/TextGamepadShotTextureHandle.h"
#include "BaseTextureHandles/TextGamepadTimeStopTextureHandle/TextGamepadTimeStopTextureHandle.h"
#include "BaseTextureHandles/TextGamepadPoseTextureHandle/TextGamepadPoseTextureHandle.h"

#include "BaseTextureHandles/TextKeyboardStageSelectTextureHandle/TextKeyboardStageSelectTextureHandle.h"
#include "BaseTextureHandles/TextkeyboardStartTextureHandle/TextkeyboardStartTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardMoveTextureHandle/TextKeyboardMoveTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardShotTextureHandle/TextKeyboardShotTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardTimeStopTextureHandle/TextKeyboardTimeStopTextureHandle.h"
#include "BaseTextureHandles/TextKeyboardPoseTextureHandle/TextKeyboardPoseTextureHandle.h"

#include "BaseTextureHandles/TextStageServiceRailwayTextureHandle/TextStageServiceRailwayTextureHandle.h"
#include "BaseTextureHandles/TextStageEfforTownTextureHandle/TextStageEfforTownTextureHandle.h"
#include "BaseTextureHandles/TextStageTechnoRootTextureHandle/TextStageTechnoRootTextureHandle.h"
#include "BaseTextureHandles/TextStageImaginaryPlamTextureHandle/TextStageImaginaryPlamTextureHandle.h"

#include "BaseTextureHandles/GameClearTextureHandle/GameClearTextureHandle.h"
#include "BaseTextureHandles/GameOverTextureHandle/GameOverTextureHandle.h"

#include "BaseTextureHandles/PoseTextureHandle/PoseTextureHandle.h"
#include "BaseTextureHandles/PoseReturnGameTextureHandle/PoseReturnGameTextureHandle.h"
#include "BaseTextureHandles/PoseEndGameTextureHandle/PoseEndGameTextureHandle.h"

class TextureHandleStore
{
public:

	// ハンドルネーム
	enum HandleName
	{
		kWhite,
		kReticle,
		kTitle,

		kPose,
		kPoseReturnGame,
		kPoseEndGame,

		kStage1,
		kStage2,
		kStage3,

		kGamepadStageSelect,
		kGamepadStart,
		kGamepadMove,
		kGamepadShot,
		kGamepadTimeStop,
		kGamepadPose,

		kKeyboardStageSelect,
		kKeyboardStart,
		kKeyboardMove,
		kKeyboardShot,
		kKeyboardTimeStop,
		kKeyboardPose,

		kStageServiceRailway,
		kStageEfforTown,
		kStageTechnoRoot,
		kStageImaginaryPlam,

		kGameClear,
		kGameOver,

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

