#include "Pose.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void Pose::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d , const TextureHandleStore* textureHandleStore)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(textureHandleStore);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	textureHandleStore_ = textureHandleStore;

	// 2Dカメラの生成ト初期化
	camera2d_ = std::make_unique<Camera2D>();
	camera2d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// ポーズの箱の生成と初期化
	poseBox_ = std::make_unique<PoseBox>();
	poseBox_->Initialize(engine_ , camera2d_.get());

	// ポーズの背景の生成と初期化
	poseBg_ = std::make_unique<PoseBg>();
	poseBg_->Initialize(engine_, camera2d_.get());

	// スプライトの生成
	spritePose_ = std::make_unique<Sprite>();
	spritePose_->Initialize(engine_, camera2d_.get(), textureHandleStore_->GetTextureHandle(TextureHandleStore::kPose));
	spritePose_->worldTransform_->translation_ = Vector3(static_cast<float>(engine_->GetScreenWidth() / 2), 200.0f, 0.0f);
	spritePose_->worldTransform_->scale_ *= 2.0f;

	// スプライトの生成
	spriteReturnGame_ = std::make_unique<Sprite>();
	spriteReturnGame_->Initialize(engine_, camera2d_.get(), textureHandleStore_->GetTextureHandle(TextureHandleStore::kPoseReturnGame));
	spriteReturnGame_->worldTransform_->translation_ = Vector3(static_cast<float>(engine_->GetScreenWidth() / 2), 400.0f, 0.0f);

	// スプライトの生成
	spriteEndGame_ = std::make_unique<Sprite>();
	spriteEndGame_->Initialize(engine_, camera2d_.get(), textureHandleStore_->GetTextureHandle(TextureHandleStore::kPoseEndGame));
	spriteEndGame_->worldTransform_->translation_ = Vector3(static_cast<float>(engine_->GetScreenWidth() / 2), 550.0f, 0.0f);


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

	// 2Dカメラの更新
	camera2d_->UpdateMatrix();

	// ポーズボックスの更新
	poseBox_->Update();

	// ポーズ背景の更新
	poseBg_->Update();


	// フェーズの更新処理
	switch (phase_)
	{
	case kFadeIn:
		// フェードイン
		FadeInUpdate();
		
		break;

	case kOperation:
		// 操作

		Operation();

		spritePose_->Update();

		if (selectMenu == returnGame)
		{
			spriteReturnGame_->color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
			spriteEndGame_->color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (selectMenu == returnStageSelect)
		{
			spriteReturnGame_->color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			spriteEndGame_->color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		spriteEndGame_->Update();
		spriteReturnGame_->Update();

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

	engine_->SetOffscreenEffect(kSmoothing);

	// ポーズ背景の描画
	poseBg_->Draw();

	// ポーズボックスの描画
	poseBox_->Draw();

	if (phase_ == kOperation)
	{
		spritePose_->Draw();
		spriteEndGame_->Draw();
		spriteReturnGame_->Draw();
	}
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

	// ポーズボックスが開く
	poseBox_->OpenReset(kMaxParameterFadeIn);

	// ポーズ背景が色づく
	poseBg_->ColoredReset(kMaxParameterFadeIn);
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
/// 操作
/// </summary>
void Pose::Operation()
{
	if (engine_->IsGamepadEnable(0))
	{
		OperationGamepad();
	}
	else
	{
		OperationKeyboard();
	}
}

/// <summary>
/// 操作 : ゲームパッド
/// </summary>
void Pose::OperationGamepad()
{

	if (engine_->IsGamepadEnable(0))
	{
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_START))
		{
			PoseReleaseButton();
			return;
		}
	}



	// デッドゾーン
	float deadZone = 0.7f;

	if (engine_->GetGamepadLeftStick(0).y <= -deadZone)
	{
		if (selectMenu < returnStageSelect)
		{
			selectMenu++;
		}
	}

	if (engine_->GetGamepadLeftStick(0).y >= deadZone)
	{
		if (selectMenu > returnGame)
		{
			selectMenu--;
		}
	}

	if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_A))
	{
		switch (selectMenu)
		{
		case returnGame:

			PoseReleaseButton();
			return;

			break;

		case returnStageSelect:

			isEndGame_ = true;
			return;

			break;
		}
	}
}

/// <summary>
/// 操作 : キーボード
/// </summary>
void Pose::OperationKeyboard()
{
	if (engine_->GetKeyTrigger(DIK_S) || engine_->GetKeyTrigger(DIK_DOWN))
	{
		if (selectMenu < returnStageSelect)
		{
			selectMenu++;
		}
	}

	if (engine_->GetKeyTrigger(DIK_W) || engine_->GetKeyTrigger(DIK_UP))
	{
		if (selectMenu > returnGame)
		{
			selectMenu--;
		}
	}

	if (engine_->GetKeyTrigger(DIK_SPACE))
	{
		switch (selectMenu)
		{
		case returnGame:

			PoseReleaseButton();
			return;

			break;

		case returnStageSelect:

			isEndGame_ = true;
			return;

			break;
		}
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

	// ポーズボックスが閉じる
	poseBox_->CloseReset(kMaxParameterFadeOut);

	// ポーズ背景が消える
	poseBg_->ColorFabesReset(kMaxParameterFadeIn);
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