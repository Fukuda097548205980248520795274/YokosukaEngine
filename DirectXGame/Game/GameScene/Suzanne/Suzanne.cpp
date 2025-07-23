#include "Suzanne.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Suzanne::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
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
	worldTransform_->translation_.x = -5.0f;

	// UVトランスフォームの生成と初期化
	std::unique_ptr uvTransform = std::make_unique<UvTransform>();
	uvTransform->Initialize();
	uvTransforms_.push_back(std::move(uvTransform));

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/suzanne", "suzanne.obj");


	// テクスチャを読み込む
	ghUvChecker_ = engine_->LoadTexture("./Resources/Textures/uvChecker.png");
}

/// <summary>
/// 更新処理
/// </summary>
void Suzanne::Update()
{
	if (ImGui::TreeNode("Suzanne"))
	{
		ImGui::DragFloat3("scale", &worldTransform_->scale_.x, 0.1f);
		ImGui::DragFloat3("rotation", &worldTransform_->rotation_.x, 0.01f);
		ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
		ImGui::Text("\n");
		ImGui::DragFloat2("uvScale", &uvTransforms_[0]->scale_.x, 0.1f);
		ImGui::DragFloat("uvRotation", &uvTransforms_[0]->rotation_.z, 0.01f);
		ImGui::DragFloat2("uvTranslation", &uvTransforms_[0]->translation_.x, 0.1f);
		ImGui::TreePop();
	}

	if (engine_->IsGamepadEnable(0))
	{
		// カメラの向きで移動方向を変える
		Matrix4x4 rotateMatrix = MakeRotateMatrix(camera3d_->rotation_);
		Vector3 velocity = Transform(Vector3(engine_->GetGamepadLeftStick(0).x, engine_->GetGamepadLeftStick(0).y, 0.0f), rotateMatrix) * 0.5f;

		worldTransform_->translation_ += velocity;

		if (engine_->GetGamepadButtonPress(0, Gamepad_ShoulderL))
		{
			worldTransform_->rotation_.y += 0.03f;
		}
		else if(engine_->GetGamepadButtonPress(0,Gamepad_ShoulderR))
		{
			worldTransform_->rotation_.y -= 0.03f;
		}
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
void Suzanne::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransforms_, camera3d_, modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}