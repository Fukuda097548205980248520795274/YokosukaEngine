#include "TutorialGroundEmpty.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void TutorialGroundEmpty::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラス初期化
	StageObject::Initialize(engine, camera3d , position);


	// モデルを読み込む
	groundModelHandle_ = engine_->LoadModelData("./Resources/Models/stageObject/tutorial/ground/tutorial_ground_empty", "tutorial_ground_empty.obj");

	// ワールドトランスフォームの生成と初期化
	groundWorldTrasnform_ = std::make_unique<WorldTransform>();
	groundWorldTrasnform_->Initialize();
	groundWorldTrasnform_->SetParent(worldTransform_.get());

	// UVトランスフォームの生成と初期化
	groundUvTransform_ = std::make_unique<UvTransform>();
	groundUvTransform_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void TutorialGroundEmpty::Update()
{
	// 基底クラス更新
	StageObject::Update();

	// 地面のトランスフォームの更新
	groundWorldTrasnform_->UpdateWorldMatrix();
	groundUvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void TutorialGroundEmpty::Draw()
{
	// 地面を描画する
	engine_->DrawModel(groundWorldTrasnform_.get(), groundUvTransform_.get(), camera3d_, groundModelHandle_, Vector4(0.0f, 0.0f, 0.0f, 1.0f), true);

}