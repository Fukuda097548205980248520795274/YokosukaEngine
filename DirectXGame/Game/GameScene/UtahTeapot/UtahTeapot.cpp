#include "UtahTeapot.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void UtahTeapot::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
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
	worldTransform_->translation_.x = -15.0f;

	// UVトランスフォームの生成と初期化
	std::unique_ptr uvTransform = std::make_unique<UvTransform>();
	uvTransform->Initialize();
	uvTransforms_.push_back(std::move(uvTransform));

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/teapot", "teapot.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void UtahTeapot::Update()
{
	if (ImGui::TreeNode("UtahTeapot"))
	{
		ImGui::DragFloat3("scale", &worldTransform_->scale_.x, 0.1f);
		ImGui::DragFloat3("rotation", &worldTransform_->rotation_.x, 0.01f);
		ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
		ImGui::Text("\n");
		ImGui::ColorEdit4("color", &color_.x);
		ImGui::DragFloat2("uvScale", &uvTransforms_[0]->scale_.x, 0.1f);
		ImGui::DragFloat("uvRotation", &uvTransforms_[0]->rotation_.z, 0.01f);
		ImGui::DragFloat2("uvTranslation", &uvTransforms_[0]->translation_.x, 0.1f);
		ImGui::TreePop();
	}

	worldTransform_->rotation_.y += 0.01f;
	uvTransforms_[0]->translation_.x += 0.01f;

	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	for (std::unique_ptr<UvTransform>& uvTransform : uvTransforms_)
	{
		uvTransform->UpdateWorldMatrix();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void UtahTeapot::Draw(bool enableLighting, bool enableHalfLambert)
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransforms_, camera3d_, modelHandle_, color_, enableLighting, enableHalfLambert);
}