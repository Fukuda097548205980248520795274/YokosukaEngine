#include "TitleScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void TitleScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// フェーズ初期化
	phase_ = Phase::kFadeOut;


	// 3Dカメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// メインカメラの生成と初期化
	mainCamera_ = std::make_unique<MainCamera>();
	mainCamera_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));
	mainCamera_->SetCameraPosition(Vector3(0.0f, 0.0f, -15.0f));

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLight_->direction_ = Normalize(Vector3{ 0.0f , -1.0f , 1.0f });

	// デバッグカメラの表示の初期化
#ifdef _DEBUG

	// 軸方向表示の生成と初期化
	axis_ = std::make_unique<Axis>();
	axis_->Initialize(engine_);

#endif

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());

	// ゲームタイトルの生成と初期化
	gameTitle_ = std::make_unique<GameTitle>();
	gameTitle_->Initialize(engine_, camera3d_.get(), directionalLight_.get());

	// タイトル用のプレイヤーの生成と初期化
	titlePlayer_ = std::make_unique<TitlePlayer>();
	titlePlayer_->Initialize(engine_, camera3d_.get(), directionalLight_.get());

	// フェードの生成と初期化
	fade_ = std::make_unique<Fade>();
	fade_->Initialize(engine_);

	// フェードアウトする
	fade_->Start(Fade::Status::kFadeOut, kFadeTime);
}

/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update()
{
	// ゲームカメラ・デバッグカメラ切り替え
#ifdef _DEBUG

	// Pキーで、カメラを切り替える
	if (engine_->GetKeyTrigger(DIK_P))
	{
		if (isDebugCameraActive_ == false)
		{
			isDebugCameraActive_ = true;
		} else
		{
			isDebugCameraActive_ = false;
		}
	}

	// 軸方向表示の更新
	axis_->Update(camera3d_->rotation_);

#endif

	// カメラの値を渡して更新　ゲームカメラ
	if (isDebugCameraActive_ == false)
	{
		// ゲームカメラの更新
		mainCamera_->Update();
		camera3d_->UpdateOthersCameraData(mainCamera_->GetGameCameraInstance());
	} else
	{
		// デバッグカメラの更新
		engine_->DebugCameraUpdate();
		camera3d_->UpdateDebugCameraData(engine_->GetDebugCameraInstance());
	}

	// 天球を更新する
	skydome_->Update();

	// ゲームタイトルを更新する
	gameTitle_->Update();

	// タイトル用のプレイヤー
	titlePlayer_->Update();


	// フェーズの更新処理
	switch (phase_)
	{
	case Phase::kMain:
		// メイン部

		MainUpdate();

		break;
	}

	// フェーズ切り替え
	ChangePhase();

	// フェードの更新
	fade_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void TitleScene::Draw()
{
	// デバッグカメラの表示
#ifdef _DEBUG

	// 軸方向表示の描画
	axis_->Draw();

#endif

	// 天球を描画する
	skydome_->Draw();

	// ゲームタイトルを描画する
	gameTitle_->Draw();

	// タイトル用のプレイヤーを描画する
	titlePlayer_->Draw();

	// フェードの描画
	fade_->Draw();
}

/// <summary>
/// フェーズの状態遷移
/// </summary>
void TitleScene::ChangePhase()
{
	switch (phase_)
	{
	case Phase::kFadeOut:
		// フェードアウト

		// フェードが終了したら、メイン部に遷移する
		if (fade_->IsFinished())
		{
			phase_ = Phase::kMain;
		}

		break;

	case Phase::kMain:
		// メイン部

		break;

	case Phase::kFadeIn:
		// フェードイン

		// フェードが終了したら、タイトルシーンを終了させる
		if (fade_->IsFinished())
		{
			isFinished_ = true;
		}

		break;
	}
}

/// <summary>
/// メイン部の更新処理
/// </summary>
void TitleScene::MainUpdate()
{
	// Aボタンで、フェードインに移行する
	if (engine_->IsGamepadEnable(0))
	{
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_A) || engine_->GetKeyTrigger(DIK_SPACE))
		{
			phase_ = Phase::kFadeIn;
			fade_->Start(Fade::Status::kFadeIn, kFadeTime);
		}
	}
	else
	{
		// スペースキーで、フェードインに移行する
		if (engine_->GetKeyTrigger(DIK_SPACE))
		{
			phase_ = Phase::kFadeIn;
			fade_->Start(Fade::Status::kFadeIn, kFadeTime);
		}
	}
}