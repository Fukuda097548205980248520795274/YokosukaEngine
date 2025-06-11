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

#endif


	// ワールドトランスフォームの生成と初期化
	worldTransform1_ = std::make_unique<WorldTransform>();
	worldTransform1_->Initialize();

	// UVトランスフォームの生成と初期化
	uvTransform1_ = std::make_unique<UvTransform>();
	uvTransform1_->Initialize();

	// ワールドトランスフォームの生成と初期化
	worldTransform2_ = std::make_unique<WorldTransform>();
	worldTransform2_->Initialize();
	worldTransform2_->translation_.y += 2.0f;

	// UVトランスフォームの生成と初期化
	uvTransform2_ = std::make_unique<UvTransform>();
	uvTransform2_->Initialize();


	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/StanfordBunny", "StanfordBunny.obj");

	// テクスチャを読み込む
	textureHandle_ = engine->LoadTexture("./Resources/Textures/uvChecker.png");

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;
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




	// トランスフォームを更新する
	worldTransform1_->UpdateWorldMatrix();
	uvTransform1_->UpdateWorldMatrix();

	// トランスフォームを更新する
	worldTransform2_->UpdateWorldMatrix();
	uvTransform2_->UpdateWorldMatrix();
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

#endif

	engine_->DrawSphere(worldTransform1_.get(), uvTransform1_.get(), camera3d_.get(), textureHandle_, { 1.0f , 1.0f , 1.0f , 1.0f },
		directionalLight_.get() , pointLight_.get() , spotLight_.get());

	engine_->DrawSphere(worldTransform2_.get(), uvTransform2_.get(), camera3d_.get(), textureHandle_, { 1.0f , 0.0f , 0.0f , 1.0f },
		directionalLight_.get(), pointLight_.get(), spotLight_.get());
}