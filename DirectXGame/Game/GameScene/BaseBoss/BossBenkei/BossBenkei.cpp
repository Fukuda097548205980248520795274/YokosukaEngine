#include "BossBenkei.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void BossBenkei::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラス初期化
	BaseBoss::Initialize(engine, camera3d, position);

	worldTransform_->translation_.z = 20.0f;
	worldTransform_->translation_.y = 5.0f;
	worldTransform_->scale_ *= 3.0f;
	worldTransform_->rotation_.y = std::numbers::pi_v<float>;


	// 胴体の生成と初期化
	models_[kBody].worldTransform = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform->Initialize();
	models_[kBody].worldTransform->SetParent(worldTransform_.get());
	models_[kBody].uvTransform = std::make_unique<UvTransform>();
	models_[kBody].uvTransform->Initialize();
	models_[kBody].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/body", "body.obj");

	// 鎧の生成と初期化
	models_[kArmor].worldTransform = std::make_unique<WorldTransform>();
	models_[kArmor].worldTransform->Initialize();
	models_[kArmor].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kArmor].uvTransform = std::make_unique<UvTransform>();
	models_[kArmor].uvTransform->Initialize();
	models_[kArmor].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/armor", "armor.obj");

	// 頭の生成と初期化
	models_[kHead].worldTransform = std::make_unique<WorldTransform>();
	models_[kHead].worldTransform->Initialize();
	models_[kHead].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kHead].uvTransform = std::make_unique<UvTransform>();
	models_[kHead].uvTransform->Initialize();
	models_[kHead].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/head", "head.obj");

	// 帽子の生成と初期化
	models_[kHat].worldTransform = std::make_unique<WorldTransform>();
	models_[kHat].worldTransform->Initialize();
	models_[kHat].worldTransform->SetParent(models_[kHead].worldTransform.get());
	models_[kHat].uvTransform = std::make_unique<UvTransform>();
	models_[kHat].uvTransform->Initialize();
	models_[kHat].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/hat", "hat.obj");

	// 右肩の生成と初期化
	models_[kShoulderR].worldTransform = std::make_unique<WorldTransform>();
	models_[kShoulderR].worldTransform->Initialize();
	models_[kShoulderR].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kShoulderR].uvTransform = std::make_unique<UvTransform>();
	models_[kShoulderR].uvTransform->Initialize();
	models_[kShoulderR].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/shoulderR", "shoulderR.obj");

	// 左肩の生成と初期化
	models_[kShoulderL].worldTransform = std::make_unique<WorldTransform>();
	models_[kShoulderL].worldTransform->Initialize();
	models_[kShoulderL].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kShoulderL].uvTransform = std::make_unique<UvTransform>();
	models_[kShoulderL].uvTransform->Initialize();
	models_[kShoulderL].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/shoulderL", "shoulderL.obj");

	// 右ひじの生成と初期化
	models_[kElbowR].worldTransform = std::make_unique<WorldTransform>();
	models_[kElbowR].worldTransform->Initialize();
	models_[kElbowR].worldTransform->SetParent(models_[kShoulderR].worldTransform.get());
	models_[kElbowR].uvTransform = std::make_unique<UvTransform>();
	models_[kElbowR].uvTransform->Initialize();
	models_[kElbowR].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/elbowR", "elbowR.obj");

	// 左ひじの生成と初期化
	models_[kElbowL].worldTransform = std::make_unique<WorldTransform>();
	models_[kElbowL].worldTransform->Initialize();
	models_[kElbowL].worldTransform->SetParent(models_[kShoulderL].worldTransform.get());
	models_[kElbowL].uvTransform = std::make_unique<UvTransform>();
	models_[kElbowL].uvTransform->Initialize();
	models_[kElbowL].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/elbowL", "elbowL.obj");

	// 右手の生成と初期化
	models_[kHandR].worldTransform = std::make_unique<WorldTransform>();
	models_[kHandR].worldTransform->Initialize();
	models_[kHandR].worldTransform->SetParent(models_[kElbowR].worldTransform.get());
	models_[kHandR].uvTransform = std::make_unique<UvTransform>();
	models_[kHandR].uvTransform->Initialize();
	models_[kHandR].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/handR", "handR.obj");

	// 左手の生成と初期化
	models_[kHandL].worldTransform = std::make_unique<WorldTransform>();
	models_[kHandL].worldTransform->Initialize();
	models_[kHandL].worldTransform->SetParent(models_[kElbowL].worldTransform.get());
	models_[kHandL].uvTransform = std::make_unique<UvTransform>();
	models_[kHandL].uvTransform->Initialize();
	models_[kHandL].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/handL", "handL.obj");

	// 武器の生成と初期化
	models_[kWeapon].worldTransform = std::make_unique<WorldTransform>();
	models_[kWeapon].worldTransform->Initialize();
	models_[kWeapon].worldTransform->SetParent(models_[kHandR].worldTransform.get());
	models_[kWeapon].uvTransform = std::make_unique<UvTransform>();
	models_[kWeapon].uvTransform->Initialize();
	models_[kWeapon].modelHandle = engine_->LoadModelData("./Resources/Models/boss/benkei/weapon", "weapon.obj");

	// 初期位置を設定する
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		models_[i].worldTransform->translation_ = kStartPosition[i];

		models_[i].worldTransform->rotation_ = kSSweepAwayStanceRotation[i];
	}
}

/// <summary>
/// 更新処理
/// </summary>
void BossBenkei::Update()
{
	ImGui::Begin("rotate");
	ImGui::DragFloat3("body", &models_[kBody].worldTransform->rotation_.x, 0.01f);
	ImGui::DragFloat3("head", &models_[kHead].worldTransform->rotation_.x, 0.01f);
	ImGui::DragFloat3("shoulderR", &models_[kShoulderR].worldTransform->rotation_.x, 0.01f);
	ImGui::DragFloat3("elbowR", &models_[kElbowR].worldTransform->rotation_.x, 0.01f);
	ImGui::DragFloat3("headR", &models_[kHandR].worldTransform->rotation_.x, 0.01f);
	ImGui::End();

	// 基底クラス更新処理
	BaseBoss::Update();

	// モデル構造体を更新する
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		models_[i].worldTransform->UpdateWorldMatrix();
		models_[i].uvTransform->UpdateWorldMatrix();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void BossBenkei::Draw()
{
	// モデル構造体を描画する
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		engine_->DrawModel(models_[i].worldTransform.get(), models_[i].uvTransform.get(), camera3d_, models_[i].modelHandle,
			models_[i].color, true);
	}
}




/// <summary>
/// 攻撃ビヘイビア : 薙ぎ払い : 初期化
/// </summary>
void BossBenkei::AttackBehaviorSweepAwayInitialize()
{
	sweepAwayParamter_ = 0.0f;
}

/// <summary>
/// 攻撃ビヘイビア : 薙ぎ払い : 更新処理
/// </summary>
void BossBenkei::AttackBehaviorSweppAwayUpdate()
{

}