#include "Game.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void Game::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert (engine != nullptr);

	// 引数を受け取る
	engine_ = engine;

	// 3Dカメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// 2Dカメラの生成と初期化
	camera2d_ = std::make_unique<Camera2D>();
	camera2d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// ゲームカメラの生成と初期化
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));


	// デバッグカメラの表示の初期化
#ifdef _DEBUG

	debugCameraWorldTransform_ = std::make_unique<WorldTransform>();
	debugCameraWorldTransform_->Initialize();
	debugCameraWorldTransform_->scale_ = { 144.0f , 16.0f , 1.0f };
	debugCameraWorldTransform_->translation_ = 
	{ static_cast<float>(engine_->GetScreenWidth()) - debugCameraWorldTransform_->scale_.x , debugCameraWorldTransform_->scale_.y , 0.0f };

	debugCameraUvTransform_ = std::make_unique<UvTransform>();
	debugCameraUvTransform_->Initialize();

	debugCameraModelHandle_ = engine_->LoadTexture("./Resources/Textures/DebugCameraOn.png");

	// 軸方向表示の生成と初期化
	axis_ = std::make_unique<Axis>();
	axis_->Initialize(engine_);

#endif

}

/// <summary>
/// 更新処理
/// </summary>
void Game::Update()
{
	// ゲームカメラ・デバッグカメラ切り替え
#ifdef _DEBUG

	// Pキーで、カメラを切り替える
	if (engine_->GetKeyTrigger(DIK_P))
	{
		if (isDebugCameraActive_ == false)
		{
			isDebugCameraActive_ = true;
		}
		else
		{
			isDebugCameraActive_ = false;
		}
	}

	debugCameraWorldTransform_->UpdateWorldMatrix();
	debugCameraUvTransform_->UpdateWorldMatrix();

	// 軸方向表示の更新
	axis_->Update(camera3d_->rotation_);

#endif

	// カメラの値を渡して更新　ゲームカメラ
	if (isDebugCameraActive_ == false)
	{
		// ゲームカメラの更新
		gameCamera_->Update();
		camera3d_->UpdateOthersCameraData(gameCamera_->GetGameCameraInstance());
	}
	else
	{
		// デバッグカメラの更新
		engine_->DebugCameraUpdate();
		camera3d_->UpdateDebugCameraData(engine_->GetDebugCameraInstance());
	}

	// 2Dカメラを更新
	camera2d_->UpdateMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{
	// デバッグカメラの表示
#ifdef _DEBUG

	if (isDebugCameraActive_)
	{
		engine_->DrawSprite(debugCameraWorldTransform_.get(), debugCameraUvTransform_.get(),
			camera2d_.get(), debugCameraModelHandle_, { 1.0f , 1.0f , 1.0f ,1.0f });

		// グリッドを描画する
		engine_->DrawGrid(camera3d_.get());
	}

	// 軸方向表示の描画
	axis_->Draw();

#endif


}