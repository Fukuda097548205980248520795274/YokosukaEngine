#include "TutorialGroundLight.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void TutorialGroundLight::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラス初期化
	StageObject::Initialize(engine, camera3d, position);


	// ワールドトランスフォームの生成と初期化
	lightWorldTrasnform_ = std::make_unique<WorldTransform>();
	lightWorldTrasnform_->Initialize();
	lightWorldTrasnform_->SetParent(worldTransform_.get());
	lightWorldTrasnform_->scale_ = Vector3(30.0f, 0.0f, 10.0f);

	// UVトランスフォームの生成と初期化
	lightUvTransform_ = std::make_unique<UvTransform>();
	lightUvTransform_->Initialize();

	// テクスチャの読み込み
	lightTextureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void TutorialGroundLight::Update()
{
	// 基底クラス更新
	StageObject::Update();

	// 地面のトランスフォームの更新
	lightWorldTrasnform_->UpdateWorldMatrix();
	lightUvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void TutorialGroundLight::Draw()
{
	// 地面を描画する
	engine_->DrawPlane(lightWorldTrasnform_.get(), lightUvTransform_.get(), camera3d_, lightTextureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);

}