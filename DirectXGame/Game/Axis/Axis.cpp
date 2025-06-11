#include "Axis.h"

/// <summary>
/// 初期化
/// </summary>
void Axis::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;


	// カメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// ビューポート行列
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f,
		static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()), 0.0f, 1.0f);

	// ビュープロジェクションビューポート行列
	Matrix4x4 viewProjectionViewportMatrix = Multiply(Multiply(camera3d_->viewMatrix_, camera3d_->projectionMatrix_), viewportMatrix);

	// 逆行列にする
	Matrix4x4 InverseViewProjectionViewportMatrix = MakeInverseMatrix(viewProjectionViewportMatrix);

	// スクリーン座標
	Vector3 screen = { static_cast<float>(engine_->GetScreenWidth()) - 64.0f , 64.0f , 0.0f };

	// ワールド座標に変換する
	Vector3 world = Transform(screen, InverseViewProjectionViewportMatrix);


	worldTransform_->translation_ = world;
	worldTransform_->scale_ = { 0.0025f , 0.0025f , 0.0025f };


	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Axis", "Axis.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void Axis::Update(struct Vector3& cameraRotation)
{

	worldTransform_->rotation_ = {-cameraRotation.x,-cameraRotation.y, -cameraRotation.z};

	// トランスフォームを更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Axis::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, { 1.0f , 1.0f , 1.0f ,1.0f });
}