#include "Pose.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void Pose::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;


	// 効果音を読み込む
	shPose_ = engine_->LoadSound("./Resources/Sounds/Se/pose/pose.mp3");
	shPoseRelease_ = engine_->LoadSound("./Resources/Sounds/Se/pose/poseRelease.mp3");
}

/// <summary>
/// 更新処理
/// </summary>
void Pose::Update()
{
	// ポーズしていないときは処理しない
	if (isPose_ == false)
		return;

	// フェーズの更新処理
	switch (phase_)
	{
	case kFadeIn:
		// フェードイン
		FadeInUpdate();
		
		break;

	case kOperation:
		// 操作
		
		if (engine_->IsGamepadEnable(0))
		{
			if (engine_->GetGamepadButtonTrigger(0,XINPUT_GAMEPAD_START))
			{
				PoseReleaseButton();
			}
		}

		break;

	case kFadeOut:
		// フェードアウト
		FadeOutUpdate();

		break;
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Pose::Draw()
{
	// ポーズしていないときは処理しない
	if (isPose_ == false)
		return;
}

/// <summary>
/// ポーズボタン
/// </summary>
void Pose::PoseButton()
{
	if (isPose_ == false)
	{
		isPose_ = true;

		// ポーズ音
		engine_->PlaySoundData(shPose_, 0.5f);

		// フェードインの初期化
		FadeInInitialize();
	}
}

/// <summary>
/// ポーズ解除ボタン
/// </summary>
void Pose::PoseReleaseButton()
{
	if (isPose_)
	{
		if (phase_ = kOperation)
		{
			// ポーズ解除音
			engine_->PlaySoundData(shPoseRelease_, 0.5f);

			// フェードアウトの初期化
			FadeOutInitialize();
		}
	}

}


/// <summary>
/// ポーズの初期化
/// </summary>
void Pose::PoseInitialize()
{
	// フェーズ
	phase_ = kFadeIn;
}




/// <summary>
/// フェードインの初期化
/// </summary>
void Pose::FadeInInitialize()
{
	//　フェードインに移行
	phase_ = kFadeIn;

	// フェードインパラメータ
	parameterFadeIn_ = 0.0f;
}

/// <summary>
/// フェードインの更新処理
/// </summary>
void Pose::FadeInUpdate()
{
	// パラメータを進める
	parameterFadeIn_ += 1.0f / 60.0f;
	parameterFadeIn_ = std::min(parameterFadeIn_, kMaxParameterFadeIn);

	// 最大値を越えたら操作に移行する
	if (parameterFadeIn_ >= kMaxParameterFadeIn)
	{
		phase_ = kOperation;
	}
}


/// <summary>
/// フェードアウトの初期化
/// </summary>
void Pose::FadeOutInitialize()
{
	//　フェードアウトに移行
	phase_ = kFadeOut;

	// フェードアウトパラメータ
	parameterFadeOut_ = 0.0f;
}

/// <summary>
/// フェードアウトの更新処理
/// </summary>
void Pose::FadeOutUpdate()
{
	// パラメータを進める
	parameterFadeOut_ += 1.0f / 60.0f;
	parameterFadeOut_ = std::min(parameterFadeOut_, kMaxParameterFadeOut);

	// 最大値を越えたらポーズを終了する
	if (parameterFadeOut_ >= kMaxParameterFadeOut)
	{
		isPose_ = false;
	}
}