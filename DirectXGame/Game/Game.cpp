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
	camera3d_->Initialize(1280.0f , 720.0f);

	// 2Dカメラの生成と初期化
	camera2d_ = std::make_unique<Camera2D>();
	camera2d_->Initialize(1280.0f , 720.0f);

	// ゲームカメラの生成と初期化
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize(1280.0f , 720.0f);


	// デバッグカメラの表示の初期化
#ifdef _DEBUG

	debugCameraWorldTransform_ = std::make_unique<WorldTransform>();
	debugCameraWorldTransform_->Initialize();
	debugCameraWorldTransform_->scale_ = { 144.0f , 16.0f , 1.0f };
	debugCameraWorldTransform_->translation_ = { 1280.0f - debugCameraWorldTransform_->scale_.x , debugCameraWorldTransform_->scale_.y , 0.0f };

	debugCameraUvTransform_ = std::make_unique<UvTransform>();
	debugCameraUvTransform_->Initialize();

	debugCameraModelHandle_ = engine_->LoadTexture("./Resources/Textures/DebugCameraOn.png");

#endif


	
	// オブジェクトの生成と初期化
	object_ = std::make_unique<Object>();
	object_->Initialize(engine_, camera3d_.get());

	// スプライトの生成と初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(engine_, camera2d_.get());

	// 音源
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Scream_A10.mp3");
	engine_->PlayerSoundData(soundHandle_, 0.3f);
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



	// オブジェクト更新
	object_->Update();

	// スプライトの更新
	sprite_->Update();
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
	}

#endif

	// オブジェクト描画
	object_->Draw();

	// スプライトの描画
	sprite_->Draw();
}