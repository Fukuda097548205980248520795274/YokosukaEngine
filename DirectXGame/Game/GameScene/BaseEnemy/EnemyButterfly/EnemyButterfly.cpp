#include "EnemyButterfly.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void EnemyButterfly::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラス初期化
	BaseEnemy::Initialize(engine, camera3d, position);
	worldTransform_->scale_ *= 2.0f;

	hitSize_ = { 2.0f , 2.0f , 1.0f };
	hp_ = 50;

	// 胴体の初期化
	models_[kBody].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform_->Initialize();
	models_[kBody].worldTransform_->SetParent(worldTransform_.get());
	models_[kBody].worldTransform_->translation_ = kStartPosition[kBody];
	models_[kBody].uvTransform_ = std::make_unique<UvTransform>();
	models_[kBody].uvTransform_->Initialize();
	models_[kBody].modelHandle_ = engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/body", "body.obj");
	models_[kBody].color = Vector4(0.5f, 0.0f, 0.0f, 1.0f);

	// 右の羽の初期化
	models_[kWingR].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kWingR].worldTransform_->Initialize();
	models_[kWingR].worldTransform_->SetParent(models_[kBody].worldTransform_.get());
	models_[kWingR].worldTransform_->translation_ = kStartPosition[kWingR];
	models_[kWingR].uvTransform_ = std::make_unique<UvTransform>();
	models_[kWingR].uvTransform_->Initialize();
	models_[kWingR].modelHandle_ = engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/wingR", "wingR.obj");
	models_[kWingR].color = Vector4(0.5f, 0.5f, 1.0f, 1.0f);

	// 左の羽の初期化
	models_[kWingL].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kWingL].worldTransform_->Initialize();
	models_[kWingL].worldTransform_->SetParent(models_[kBody].worldTransform_.get());
	models_[kWingL].worldTransform_->translation_ = kStartPosition[kWingL];
	models_[kWingL].uvTransform_ = std::make_unique<UvTransform>();
	models_[kWingL].uvTransform_->Initialize();
	models_[kWingL].modelHandle_ = engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/wingL", "wingL.obj");
	models_[kWingL].color = Vector4(0.5f, 0.5f, 1.0f, 1.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterfly::Update()
{
	// 基底クラス更新
	BaseEnemy::Update();

	// モデルの更新
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		models_[i].worldTransform_->UpdateWorldMatrix();
		models_[i].uvTransform_->UpdateWorldMatrix();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void EnemyButterfly::Draw()
{
	// モデルの描画
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		engine_->DrawModel(models_[i].worldTransform_.get(), models_[i].uvTransform_.get(), camera3d_, models_[i].modelHandle_,
			models_[i].color, true);
	}
}