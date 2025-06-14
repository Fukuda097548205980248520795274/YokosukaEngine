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


	// 3Dカメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// メインカメラの生成と初期化
	mainCamera_ = std::make_unique<MainCamera>();
	mainCamera_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// デバッグカメラの表示の初期化
#ifdef _DEBUG

	// 軸方向表示の生成と初期化
	axis_ = std::make_unique<Axis>();
	axis_->Initialize(engine_);

#endif
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
}