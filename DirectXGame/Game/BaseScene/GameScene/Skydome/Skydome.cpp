#include "Skydome.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Skydome::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;


	/*---------
		中心
	----------*/

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();
	uvTransform_->scale_.y = 0.5f;

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Skydome", "Skydome.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void Skydome::Update()
{
	// UVを上にあげる
	uvTransform_->translation_.y += 0.0005f;
	if (uvTransform_->translation_.y >= 1.0f)
		uvTransform_->translation_.y = 0.0f;

	// 中心の更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Skydome::Draw()
{
	// 本体を描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
}