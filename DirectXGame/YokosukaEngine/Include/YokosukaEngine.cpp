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


	// デバッグツールの初期化
#ifdef _DEBUG

	// デバッグカメラの生成と初期化
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(this);


	/*---------------
	    軸方向表示
	---------------*/

	// 軸の生成と初期化
	axis_.camera3d = std::make_unique<Camera3D>();
	axis_.camera3d->Initialize(static_cast<float>(windowApplication_->GetWindowWidth()), static_cast<float>(windowApplication_->GetWindowHeight()));


	// ワールドトランスフォーム
	axis_.worldTransform = std::make_unique<WorldTransform>();
	axis_.worldTransform->Initialize();

	// ビューポート行列
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f,
		static_cast<float>(windowApplication_->GetWindowWidth()), static_cast<float>(windowApplication_->GetWindowHeight()), 0.0f, 1.0f);

	// ビュープロジェクションビューポート行列
	Matrix4x4 viewProjectionViewportMatrix = axis_.camera3d->viewMatrix_ * axis_.camera3d->projectionMatrix_ * viewportMatrix;

	// 逆行列にする
	Matrix4x4 InverseViewProjectionViewportMatrix = MakeInverseMatrix(viewProjectionViewportMatrix);

	// スクリーン座標
	Vector3 screen = { static_cast<float>(windowApplication_->GetWindowWidth()) - 64.0f , 64.0f , 0.0f };

	// ワールド座標に変換する
	Vector3 world = Transform(screen, InverseViewProjectionViewportMatrix);

	axis_.worldTransform->translation_ = world;
	axis_.worldTransform->scale_ = { 0.0025f , 0.0025f , 0.0025f };


	// UVトランスフォーム
	axis_.uvTransform = std::make_unique<UvTransform>();
	axis_.uvTransform->Initialize();

	axis_.modelHandle = LoadModelData("./Resources/Models/Axis", "Axis.obj");

#endif
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

/// <summary>
/// 軸方向表示の更新処理
/// </summary>
void YokosukaEngine::AxisUpdate(const Vector3& cameraRotation)const
{
	// 逆に回転させる
	axis_.worldTransform->rotation_ = { -cameraRotation.x , -cameraRotation.y , -cameraRotation.z };

	// 軸方向表示のトランスフォームを更新する
	axis_.worldTransform->UpdateWorldMatrix();
	axis_.uvTransform->UpdateWorldMatrix();
}

/// <summary>
/// 軸方向表示の描画処理
/// </summary>
void YokosukaEngine::AxisDraw()const
{
	// 軸方向表示のモデルを描画する
	DrawModel(axis_.worldTransform.get(), axis_.uvTransform.get(), axis_.camera3d.get(), axis_.modelHandle, { 1.0f , 1.0f  ,1.0f , 1.0f });
}

#endif