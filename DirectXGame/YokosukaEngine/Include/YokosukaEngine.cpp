#include "YokosukaEngine.h"

/// <summary>
/// デストラクタ
/// </summary>
YokosukaEngine::~YokosukaEngine()
{
	// AudioStore
	delete audioStore_;

	// Input
	delete input_;

	// DirectXCommon
	delete directXCommon_;

	// ログ出力
	delete log_;

	// ウィンドウアプリケーション
	delete windowApplication_;

	// リソースリークチェッカー
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}

	// COMの終了処理
	CoUninitialize();
}

/// <summary>
/// 初期化
/// </summary>
void YokosukaEngine::Initialize(const int32_t kWindowWidth, const int32_t kWindowHeight, const std::string& titleName)
{
	// 乱数を設定する
	currentTimer_ = static_cast<unsigned int>(time(nullptr));
	srand(currentTimer_);

	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// 出力用のディレクトリを用意する
	std::filesystem::create_directory("YokosukaEngine/Output");

	// CrashHandlerの登録
	SetUnhandledExceptionFilter(ExportDump);

	// ウィンドウアプリケーションの生成と初期化
	windowApplication_ = new WinApp();
	windowApplication_->Initialize(kWindowWidth, kWindowHeight, ConvertString(titleName));

	// ログ出力の生成と初期化
	log_ = new OutputLog();
	log_->Initialize();

	// DirectXCommonの生成と初期化
	directXCommon_ = new DirectXCommon();
	directXCommon_->Initialize(log_, windowApplication_);

	// Inputの生成と初期化
	input_ = new Input();
	input_->Initialize(windowApplication_);

	// AudioStoreの生成と初期化
	audioStore_ = new AudioStore();
	audioStore_->Initialize();
}


// デバッグツールメソッド
#ifdef _DEBUG

/// <summary>
/// グリッドを描画する
/// </summary>
/// <param name="camera"></param>
void YokosukaEngine::DrawGrid(const Camera3D* camera) const
{
	// グリッドの半径
	const float kGridRadius = 500.0f;

	// 分割数
	const uint32_t kSubdivisions = 200;

	// 1マスの大きさ
	const float kSquareSize = kGridRadius * 2.0f / static_cast<float>(kSubdivisions);

	for (uint32_t index = 0; index <= kSubdivisions; ++index)
	{
		// 中心線
		if (index == kSubdivisions / 2)
		{
			// X軸の赤線
			DrawLine(Vector3{ -kGridRadius , 0.0f , -kGridRadius + kSquareSize * index },
				Vector3{ kGridRadius , 0.0f , -kGridRadius + kSquareSize * index }, camera, Vector4{ 1.0f , 0.3f , 0.3f , 0.5f });

			// Y軸の赤線
			DrawLine(Vector3{ -kGridRadius + kSquareSize * index , 0.0f , -kGridRadius },
				Vector3{ -kGridRadius + kSquareSize * index , 0.0f , kGridRadius }, camera, Vector4{ 0.3f , 0.3f , 1.0f , 0.5f });
		}
		else
		{
			// それ以外の線

			if (index % 5 == 0)
			{
				DrawLine(Vector3{ -kGridRadius , 0.0f , -kGridRadius + kSquareSize * index },
					Vector3{ kGridRadius , 0.0f , -kGridRadius + kSquareSize * index }, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.5f });

				DrawLine(Vector3{ -kGridRadius + kSquareSize * index , 0.0f , -kGridRadius },
					Vector3{ -kGridRadius + kSquareSize * index , 0.0f , kGridRadius }, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.5f });
			}
			else
			{
				DrawLine(Vector3{ -kGridRadius , 0.0f , -kGridRadius + kSquareSize * index },
					Vector3{ kGridRadius , 0.0f , -kGridRadius + kSquareSize * index }, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.1f });

				DrawLine(Vector3{ -kGridRadius + kSquareSize * index , 0.0f , -kGridRadius },
					Vector3{ -kGridRadius + kSquareSize * index , 0.0f , kGridRadius }, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.1f });
			}
		}
	}
}

#endif

// デバッグツール
#ifdef _DEBUG

/*------------------
    デバッグカメラ
------------------*/

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void DebugCamera::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// 3Dカメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));
}

/// <summary>
/// 更新処理
/// </summary>
void DebugCamera::Update()
{
	// マウスホイールを上回転させると、向いている方向にズームイン
	if (engine_->GetMouseWheelUp())
	{
		// 回転行列で進む方向を取得する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(camera3d_->rotation_);
		Vector3 velocity = TransformNormal(Vector3{ 0.0f,0.0f,1.0f }, rotateMatrix);

		// 移動する
		camera3d_->translation_ += velocity;
	}

	// マウスホイールを下回転させると、向いている方向にズームアウト
	if (engine_->GetMouseWheelDown())
	{
		// 回転行列で進む方向を取得する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(camera3d_->rotation_);
		Vector3 velocity = TransformNormal(Vector3{ 0.0f,0.0f,-1.0f }, rotateMatrix);

		// 移動する
		camera3d_->translation_ += velocity;
	}

	// LShift + マウスホイールを押して動かすと、向いている方向の左右に移動できる
	if (engine_->GetKeyPress(DIK_LSHIFT) && engine_->GetMouseButtonPress(kMouseButtonCenter))
	{
		// マウスのベクトルと距離を取得し、正規化する
		Vector2 mouseVector = engine_->GetMouseVelocity();
		float mouseVectorLength = Length(mouseVector);
		mouseVector = Normalize(mouseVector);

		// 移動量制御
		const float kSpeedController = mouseVectorLength * 0.01f;

		// 回転行列で進む方向を取得する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(camera3d_->rotation_);
		Vector3 velocity = TransformNormal({ -mouseVector.x , mouseVector.y , 0.0f }, rotateMatrix);

		// 移動する
		camera3d_->translation_ += velocity * kSpeedController;
	} else if (engine_->GetMouseButtonPress(kMouseButtonCenter))
	{
		// マウスホイールを押して動かすと、向いている方向を回転できる

		// マウスのベクトルと距離を取得し、正規化する
		Vector2 mouseVector = engine_->GetMouseVelocity();
		float mouseVectorLength = Length(mouseVector);
		mouseVector = Normalize(mouseVector);

		// 回転量制御
		const float kSpeedController = mouseVectorLength * 0.001f;

		// 回転量
		Vector2 rotationVelocity = mouseVector * kSpeedController;

		// 回転させる
		camera3d_->rotation_.x += rotationVelocity.y;
		camera3d_->rotation_.y += rotationVelocity.x;
	}
}


/*---------------
    軸方向表示
---------------*/

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void AxialDirectionDisplay::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// 軸の生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));


	// ワールドトランスフォーム
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// ビューポート行列
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f,
		static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()), 0.0f, 1.0f);

	// ビュープロジェクションビューポート行列
	Matrix4x4 viewProjectionViewportMatrix = camera3d_->viewMatrix_ * camera3d_->projectionMatrix_ * viewportMatrix;

	// 逆行列にする
	Matrix4x4 InverseViewProjectionViewportMatrix = MakeInverseMatrix(viewProjectionViewportMatrix);

	// スクリーン座標
	Vector3 screen = { static_cast<float>(engine_->GetScreenWidth()) - 64.0f , 64.0f , 0.0f };

	// ワールド座標に変換する
	Vector3 world = Transform(screen, InverseViewProjectionViewportMatrix);

	worldTransform_->translation_ = world;
	worldTransform_->scale_ = { 0.0025f , 0.0025f , 0.0025f };


	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	modelHandle_ = engine_->LoadModelData("./Resources/Models/Axis", "Axis.obj");
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="cameraRotation"></param>
void AxialDirectionDisplay::Update(const Vector3& cameraRotation)
{
	// 逆に回転させる
	worldTransform_->rotation_ = { -cameraRotation.x , -cameraRotation.y , -cameraRotation.z };

	// 軸方向表示のトランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void AxialDirectionDisplay::Draw()
{
	// 軸方向表示のモデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f  ,1.0f , 1.0f });
}

#endif


/*-----------------
    メインカメラ
-----------------*/

/// <summary>
/// 初期化
/// </summary>
/// <param name="screenWidth">画面の横幅</param>
/// <param name="screenHeight">画面の縦幅</param>
void MainCamera::Initialize(float screenWidth, float screenHeight)
{
	// カメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(screenWidth, screenHeight);
}

/// <summary>
/// 更新処理
/// </summary>
void MainCamera::Update()
{
	// 3Dカメラ更新
	camera3d_->UpdateMatrix();
}


/*----------------
    シーンクラス
----------------*/

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void Scene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// 3Dカメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// 2Dカメラの生成と初期化
	camera2d_ = std::make_unique<Camera2D>();
	camera2d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// メインカメラの生成と初期化
	mainCamera_ = std::make_unique<MainCamera>();
	mainCamera_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));


	// デバッグツール
#ifdef _DEBUG

	// デバッグカメラの生成と初期化
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(engine_);

	// 軸方向表示の生成と初期化
	axialDirectoinDisplay_ = std::make_unique<AxialDirectionDisplay>();
	axialDirectoinDisplay_->Initialize(engine_);

#endif
}

/// <summary>
/// 更新処理
/// </summary>
void Scene::Update()
{
	// メインカメラ・デバッグカメラ切り替え
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

	// デバッグカメラの値を渡して更新
	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		camera3d_->UpdateOtherCamera(debugCamera_->GetCamera3DInstance());
	}

	// 軸方向表示を更新
	axialDirectoinDisplay_->Update(camera3d_->rotation_);

#endif

	// メインカメラの値を渡して更新
	if (isDebugCameraActive_ == false)
	{
		mainCamera_->Update();
		camera3d_->UpdateOtherCamera(mainCamera_->GetGameCameraInstance());
	}

	// 2Dカメラを更新
	camera2d_->UpdateMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Scene::Draw()
{
	// デバッグカメラの表示
#ifdef _DEBUG

	if (isDebugCameraActive_)
	{
		// グリッドを描画する
		engine_->DrawGrid(camera3d_.get());
	}

	// 軸方向表示を描画
	axialDirectoinDisplay_->Draw();

#endif


}