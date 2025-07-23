#include "YokosukaEngine.h"

/// <summary>
/// デストラクタ
/// </summary>
YokosukaEngine::~YokosukaEngine()
{

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
	winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize(kWindowWidth, kWindowHeight, ConvertString(titleName));

	// ロギングの生成と初期化
	logging_ = std::make_unique<Logging>();
	logging_->Initialize();

	// DirectXCommonの生成と初期化
	directXCommon_ = std::make_unique<DirectXCommon>();
	directXCommon_->Initialize(logging_.get(), winApp_.get());

	// Inputの生成と初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// AudioStoreの生成と初期化
	audioStore_ = std::make_unique<AudioStore>();
	audioStore_->Initialize();
}

/// <summary>
/// CatmullRomスプライン曲線を描画する
/// </summary>
/// <param name="controlPoints"></param>
void YokosukaEngine::DrwaCatmullRomSpline(const std::vector<Vector3>& controlPoints , const Vector4& color, const Camera3D* camera3d) const
{
	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing;

	// 線分の数
	const size_t segmentCount = 100;

	// 線分の数 + 1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; ++i)
	{
		float t = 1.0f / segmentCount * i;

		if (t >= 1.0f)
		{
			int a = 1;
		}

		Vector3 pos = CatmullRomPosition(controlPoints, t);
		pointsDrawing.push_back(pos);
	}


	std::vector<Vector3>::iterator itrPointDrawing = pointsDrawing.begin();
	itrPointDrawing += 1;

	for (; itrPointDrawing != pointsDrawing.end() ; ++itrPointDrawing)
	{
		directXCommon_->DrawLine(*(itrPointDrawing - 1), *(itrPointDrawing), camera3d, color);
	}
}

/// <summary>
/// CatmullRomの補間点を求める
/// </summary>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p3"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 YokosukaEngine::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) const
{
	const float s = 0.5f;

	float t2 = t * t;
	float t3 = t2 * t;

	Vector3 e3 = -1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3;
	Vector3 e2 = 2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3;
	Vector3 e1 = -1.0f * p0 + p2;
	Vector3 e0 = 2.0f * p1;

	return s * (e3 * t3 + e2 * t2 + e1 * t + e0);
}

/// <summary>
/// CatmullRom全体で補間点を求める
/// </summary>
/// <param name="points"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 YokosukaEngine::CatmullRomPosition(const std::vector<Vector3>& points, float t) const
{
	assert(points.size() >= 4 && "制御点は4つ以上必要です");

	// 区間数は　制御点の数 - 1
	size_t division = points.size() - 1;

	// 1区間の長さ
	float areaWidth = 1.0f / division;


	// 区間内の始点を0.0f 終点を1.0fとしたときの現在地
	float t_2 = std::fmod(t, areaWidth) * division;

	// 下限と上限の範囲に収める
	t_2 = std::clamp(t_2, 0.0f, 1.0f);


	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);

	// 区間番号が上限を超えないようにする
	index = std::min(index, division);


	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	
	// 最初の区間 p0 は p1 を重複使用する
	if (index == 0)
	{
		index0 = index1;
	}

	// 最後の区間 p3 は p2 を重複使用する
	if (index2 >= points.size())
	{
		index2 = index1;
	}

	// 最後の区間 p3 は p2 を重複使用する
	if (index3 >= points.size())
	{
		index3 = index2;
	}


	// 4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	// 4点を指定してCatmulRom補間する
	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}


// デバッグツールメソッド
#ifdef _DEBUG

/// <summary>
/// グリッドを描画する
/// </summary>
/// <param name="camera"></param>
void YokosukaEngine::DrawGrid(const Camera3D* camera , int32_t x , int32_t z) const
{
	// グリッドの半径
	const float kGridRadius = 100.0f;

	// 分割数
	const uint32_t kSubdivisions = 200;

	// 1マスの大きさ
	const float kSquareSize = kGridRadius * 2.0f / static_cast<float>(kSubdivisions);

	// 位置
	Vector3 center =
	{
		(kGridRadius * 2.0f) * x,
		0.0f,
		(kGridRadius * 2.0f) * z
	};


	for (uint32_t index = 0; index <= kSubdivisions; ++index)
	{
		// 中心線
		if (index == kSubdivisions / 2)
		{
			// X軸の赤線
			DrawLine(Vector3{ -kGridRadius , 0.0f , -kGridRadius + kSquareSize * index } + center,
				Vector3{ kGridRadius , 0.0f , -kGridRadius + kSquareSize * index } + center, camera, Vector4{ 1.0f , 0.3f , 0.3f , 0.5f });

			// Z軸の青線
			DrawLine(Vector3{ -kGridRadius + kSquareSize * index , 0.0f , -kGridRadius } + center,
				Vector3{ -kGridRadius + kSquareSize * index , 0.0f , kGridRadius } + center, camera, Vector4{ 0.3f , 0.3f , 1.0f , 0.5f });
		} 
		else
		{
			// それ以外の線

			if (index % 10 == 0)
			{
				DrawLine(Vector3{ -kGridRadius , 0.0f , -kGridRadius + kSquareSize * index } + center,
					Vector3{ kGridRadius , 0.0f , -kGridRadius + kSquareSize * index } + center, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.5f });

				DrawLine(Vector3{ -kGridRadius + kSquareSize * index , 0.0f , -kGridRadius } + center,
					Vector3{ -kGridRadius + kSquareSize * index , 0.0f , kGridRadius } + center, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.5f });
			} 
			else
			{
				DrawLine(Vector3{ -kGridRadius , 0.0f , -kGridRadius + kSquareSize * index } + center,
					Vector3{ kGridRadius , 0.0f , -kGridRadius + kSquareSize * index } + center, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.1f });

				DrawLine(Vector3{ -kGridRadius + kSquareSize * index , 0.0f , -kGridRadius } + center,
					Vector3{ -kGridRadius + kSquareSize * index , 0.0f , kGridRadius } + center, camera, Vector4{ 0.3f , 0.3f , 0.3f , 0.1f });
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

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void DebugCamera::Update()
{
	// マウスホイールを上回転させると、向いている方向にズームイン
	if (engine_->GetMouseWheelUp())
	{
		pivotPointLength_ /= 1.2f;

		// 1以下にならないようにする
		if (pivotPointLength_ < 1.0f)
			pivotPointLength_ = 1.0f;
	}

	// マウスホイールを下回転させると、向いている方向にズームアウト
	if (engine_->GetMouseWheelDown())
	{
		pivotPointLength_ *= 1.2f;
	}

	// LShift + マウスホイールを押して動かすと、向いている方向の左右に移動できる
	if (engine_->GetKeyPress(Key_ShitfL) && engine_->GetMouseButtonPress(kMouseButtonCenter))
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
		pivotPoint_ += velocity * kSpeedController * (pivotPointLength_ / 10.0f);
	}
	else if (engine_->GetMouseButtonPress(kMouseButtonCenter))
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

	camera3d_->translation_ = pivotPoint_ + SphericalCoordinate(pivotPointLength_, camera3d_->rotation_.x, -(std::numbers::pi_v<float> / 2.0f) - camera3d_->rotation_.y);


	// ワールドトランスフォームに数値を送る
	worldTransform_->scale_ = camera3d_->scale_;
	worldTransform_->translation_ = camera3d_->translation_;
	worldTransform_->rotation_ = camera3d_->rotation_;
	worldTransform_->UpdateWorldMatrix();
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
Vector3 DebugCamera::GetWorldTransform()
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
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


	// UVトランスフォームの生成と初期化
	std::unique_ptr uvTransform = std::make_unique<UvTransform>();
	uvTransform->Initialize();
	uvTransforms_.push_back(std::move(uvTransform));

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
	for (std::unique_ptr<UvTransform>& uvTransform : uvTransforms_)
	{
		uvTransform->UpdateWorldMatrix();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void AxialDirectionDisplay::Draw()
{
	// 軸方向表示のモデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransforms_, camera3d_.get(), modelHandle_, { 1.0f , 1.0f  ,1.0f , 1.0f } , false , false);
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

	// ワールドトランスフォームの生成と初期化
	pivotWorldTransform_ = std::make_unique<WorldTransform>();
	pivotWorldTransform_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void MainCamera::Update()
{
	Shake();


	// ピボットのワールドトランスフォームの更新
	pivotWorldTransform_->UpdateWorldMatrix();
	pivotPoint_ = GetPivotWorldPosition();
	
	camera3d_->translation_ = 
		pivotPoint_ + SphericalCoordinate(pivotPointLength_, camera3d_->rotation_.x, -(std::numbers::pi_v<float> / 2.0f) - camera3d_->rotation_.y);

	// シェイクを反映する
	camera3d_->translation_ += shakeMove_;
	

	// 3Dカメラ更新
	camera3d_->UpdateMatrix();
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 MainCamera::GetPivotWorldPosition()
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = pivotWorldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = pivotWorldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = pivotWorldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}

/// <summary>
/// シェイクする
/// </summary>
/// <param name="shakeTime">時間</param>
/// <param name="shakeSize">大きさ</param>
void MainCamera::SetShake(float shakeTime, float shakeSize)
{
	shakeTimer_ = shakeTime;
	shakeStartTimer_ = shakeTime;
	shakeSize_ = shakeSize;
}

/// <summary>
/// シェイクする
/// </summary>
void MainCamera::Shake()
{
	shakeMove_ = Vector3(0.0f, 0.0f, 0.0f);

	if (shakeTimer_ <= 0.0f)
		return;

	// タイマーを進める
	shakeTimer_ -= 1.0f / 60.0f;

	// 補間を求める
	float t = shakeTimer_ / shakeStartTimer_;

	Vector3 shakeVector = Vector3(float(rand() % 3) - 1, float(rand() % 3) - 1, float(rand() % 3) - 1);

	shakeMove_ = (shakeVector * t) * shakeSize_;
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

	// デバッグカメラ有効化
	isDebugCameraActive_ = true;
}

/// <summary>
/// 更新処理
/// </summary>
void Scene::Update()
{
	// メインカメラ・デバッグカメラ切り替え
#ifdef _DEBUG

	// Pキーで、カメラを切り替える
	if (engine_->GetKeyTrigger(Key_P))
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
		// デバッグカメラの位置
		

		// グリッドを描画する
		engine_->DrawGrid(camera3d_.get(), 0, 0);
	}

	// 軸方向表示を描画
	axialDirectoinDisplay_->Draw();

#endif


}