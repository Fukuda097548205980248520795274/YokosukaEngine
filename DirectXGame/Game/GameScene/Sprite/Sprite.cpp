#include "Sprite.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
void Sprite::Initialize(const YokosukaEngine* engine, const Camera2D* camera2d)
{
	// nullptrチェック
	assert(engine);
	assert(camera2d);

	// 引数を受け取る
	engine_ = engine;
	camera2d_ = camera2d;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform2D>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = { 240.0f , 135.0f , 0.0f };
	worldTransform_->scale_ = { 240.0f , 135.0f , 0.0f };

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/uvChecker.png");
}

/// <summary>
/// 更新処理
/// </summary>
void Sprite::Update()
{
	if (ImGui::TreeNode("Sprite"))
	{
		ImGui::DragFloat2("scale", &worldTransform_->scale_.x, 1.0f);
		ImGui::DragFloat("rotation", &worldTransform_->rotation_.z, 0.01f);
		ImGui::DragFloat2("translation", &worldTransform_->translation_.x, 1.0f);
		ImGui::Text("\n");
		ImGui::ColorEdit4("color", &color_.x);
		ImGui::DragFloat2("uvScale", &uvTransform_->scale_.x, 0.1f);
		ImGui::DragFloat("uvRotation", &uvTransform_->rotation_.z, 0.01f);
		ImGui::DragFloat2("uvTranslation", &uvTransform_->translation_.x, 0.1f);
		ImGui::TreePop();
	}

	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Sprite::Draw()
{
	// スプライトを描画する
	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_, textureHandle_, color_);
}