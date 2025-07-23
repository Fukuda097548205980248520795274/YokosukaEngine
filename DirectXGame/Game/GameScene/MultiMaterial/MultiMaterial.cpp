#include "MultiMaterial.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void MultiMaterial::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
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
	worldTransform_->translation_.x = 18.0f;

	// UVトランスフォームの生成と初期化
	std::unique_ptr<UvTransform> uvTransform1 = std::make_unique<UvTransform>();
	uvTransform1->Initialize();
	uvTransforms_.push_back(std::move(uvTransform1));

	std::unique_ptr<UvTransform> uvTransform2 = std::make_unique<UvTransform>();
	uvTransform2->Initialize();
	uvTransforms_.push_back(std::move(uvTransform2));

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/multiMaterial", "multiMaterial.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void MultiMaterial::Update()
{
	if (ImGui::BeginCombo("MultiMaterial", "MultiMaterial"))
	{
		ImGui::DragFloat3("scale", &worldTransform_->scale_.x, 0.1f);
		ImGui::DragFloat3("rotation", &worldTransform_->rotation_.x, 0.01f);
		ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
		ImGui::Text("\n");
		ImGui::DragFloat2("uvScale1", &uvTransforms_[0]->scale_.x, 0.1f);
		ImGui::DragFloat("uvRotation1", &uvTransforms_[0]->rotation_.z, 0.01f);
		ImGui::DragFloat2("uvTranslation1", &uvTransforms_[0]->translation_.x, 0.1f);
		ImGui::Text("\n");
		ImGui::DragFloat2("uvScale2", &uvTransforms_[1]->scale_.x, 0.1f);
		ImGui::DragFloat("uvRotation2", &uvTransforms_[1]->rotation_.z, 0.01f);
		ImGui::DragFloat2("uvTranslation2", &uvTransforms_[1]->translation_.x, 0.1f);
		ImGui::EndCombo();
	}

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
void MultiMaterial::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransforms_, camera3d_, modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}