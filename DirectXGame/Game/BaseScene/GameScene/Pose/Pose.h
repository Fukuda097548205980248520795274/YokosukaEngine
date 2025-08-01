#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class Pose
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ポーズボタン
	/// </summary>
	void PoseButton();

	/// <summary>
	/// ポーズ解除ボタン
	/// </summary>
	void PoseReleaseButton();

	/// <summary>
	/// ポーズフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsPose()const { return isPose_; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;


	// ポーズフラグ
	bool isPose_ = false;

	// フェーズ
	enum Phase
	{
		kFadeIn,
		kOperation,
		kFadeOut
	};

	// 現在のフェーズ
	Phase phase_ = kFadeIn;


	// ポーズ音
	uint32_t shPose_ = 0;

	// ポーズ解除
	uint32_t shPoseRelease_ = 0;


	/// <summary>
	/// ポーズの初期化
	/// </summary>
	void PoseInitialize();

	
	/*   フェードイン   */

	/// <summary>
	/// フェードインの初期化
	/// </summary>
	void FadeInInitialize();

	/// <summary>
	/// フェードインの更新処理
	/// </summary>
	void FadeInUpdate();

	// フェードインパラメータ
	float parameterFadeIn_ = 0.0f;

	// フェードインパラメータ最大値
	const float kMaxParameterFadeIn = 0.5f;


	/*   操作   */



	/*   フェードアウト   */

	/// <summary>
	/// フェードアウトの初期化
	/// </summary>
	void FadeOutInitialize();

	/// <summary>
	/// フェードアウトの更新処理
	/// </summary>
	void FadeOutUpdate();

	// フェードアウトパラメータ
	float parameterFadeOut_ = 0.0f;

	// フェードアウトパラメータ最大値
	float kMaxParameterFadeOut = 0.5f;
};

