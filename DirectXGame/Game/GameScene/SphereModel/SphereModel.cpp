#include "SphereModel.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void SphereModel::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_.x = 5.0f;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャハンドルを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/uvChecker.png");
}

/// <summary>
/// 更新処理
/// </summary>
void SphereModel::Update()
{
	if (ImGui::TreeNode("Sphere"))
	{
		ImGui::DragFloat3("scale", &worldTransform_->scale_.x, 0.1f);
		ImGui::DragFloat3("rotation", &worldTransform_->rotation_.x, 0.01f);
		ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
		ImGui::Text("\n");
		ImGui::ColorEdit4("color", &color_.x);
		ImGui::DragFloat2("uvScale", &uvTransform_->scale_.x, 0.1f);
		ImGui::DragFloat("uvRotation", &uvTransform_->rotation_.z, 0.01f);
		ImGui::DragFloat2("uvTranslation", &uvTransform_->translation_.x, 0.1f);
		ImGui::Text("\n");
		ImGui::SliderInt("ring", &ring, 3, 32);
		ImGui::SliderInt("segment", &segment, 3, 32);
		ImGui::TreePop();
	}

	worldTransform_->rotation_.x += 0.01f;
	uvTransform_->translation_.y += 0.01f;

	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void SphereModel::Draw(bool enableLighting, bool enableHalfLambert)
{
	// モデルを描画する
	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_, textureHandle_, segment, ring, color_, enableLighting , enableHalfLambert);
}