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


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->rotation_.y = float(std::numbers::pi);

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.gltf");

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLight_->intensity_ = 0.0f;

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->position_ = { -1.0f , 1.0f , -1.0f };
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->position_ = { 0.0f , 5.0f , 0.0f };


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



	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Game::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f ,1.0f ,1.0f },
		directionalLight_.get(), pointLight_.get(), spotLight_.get());
}