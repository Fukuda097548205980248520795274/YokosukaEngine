#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Enemy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// 基底クラス初期化
	BaseCharacter::Initialize(engine, camera3d);


	// 胴体
	models_[kBody].worldTransform = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform->Initialize();
	models_[kBody].worldTransform->SetParent(worldTransform_.get());
	models_[kBody].worldTransform->translation_ = modelsStartPosition[kBody];
	models_[kBody].uvTransform = std::make_unique<UvTransform>();
	models_[kBody].uvTransform->Initialize();
	models_[kBody].modelHandle = engine_->LoadModelData("./Resources/Models/Enemy/body", "body.obj");
	models_[kBody].color = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	// わっか
	models_[kRing].worldTransform = std::make_unique<WorldTransform>();
	models_[kRing].worldTransform->Initialize();
	models_[kRing].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kRing].worldTransform->translation_ = modelsStartPosition[kRing];
	models_[kRing].uvTransform = std::make_unique<UvTransform>();
	models_[kRing].uvTransform->Initialize();
	models_[kRing].modelHandle = engine_->LoadModelData("./Resources/Models/Enemy/ring", "ring.obj");
	models_[kRing].color = Vector4(0.3f, 0.8f, 0.3f, 1.0f);

	// 刃
	models_[kBlade].worldTransform = std::make_unique<WorldTransform>();
	models_[kBlade].worldTransform->Initialize();
	models_[kBlade].worldTransform->SetParent(models_[kRing].worldTransform.get());
	models_[kBlade].worldTransform->translation_ = modelsStartPosition[kBlade];
	models_[kBlade].uvTransform = std::make_unique<UvTransform>();
	models_[kBlade].uvTransform->Initialize();
	models_[kBlade].modelHandle = engine_->LoadModelData("./Resources/Models/Enemy/blade", "blade.obj");
	models_[kBlade].color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);

	// 回転移動ギミックの初期化
	InitializeRotateMoveGimmick();
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update()
{
	// 回転移動ギミックの更新処理
	UpdateRotateMoveGimmick();

	// 基底クラス更新処理
	BaseCharacter::Update();

	// モデルの更新処理
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		models_[i].worldTransform->UpdateWorldMatrix();
		models_[i].uvTransform->UpdateWorldMatrix();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Enemy::Draw()
{
	// モデルの描画
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		engine_->DrawModel(models_[i].worldTransform.get(), models_[i].uvTransform.get(), camera3d_,
			models_[i].modelHandle, models_[i].color, false);
	}
}


/// <summary>
/// 回転移動ギミック初期化
/// </summary>
void Enemy::InitializeRotateMoveGimmick()
{
	rotateMoveTime_ = 3.0f;
	rotateMoveParameter_ = 0.0f;
}

/// <summary>
/// 回転移動ギミック更新処理
/// </summary>
void Enemy::UpdateRotateMoveGimmick()
{
	// 1フレームでのパラメータ
	const float step = 2.0f * std::numbers::pi_v<float> / (rotateMoveTime_ * 60.0f);

	// パラメータを加算する
	rotateMoveParameter_ += step;
	rotateMoveParameter_ = std::fmod(rotateMoveParameter_, 2.0f * std::numbers::pi_v<float>);


	// 速度ベクトル
	Vector3 velocity = Normalize(Vector3(std::cos(rotateMoveParameter_), 0.0f, std::sin(rotateMoveParameter_))) * rotateMoveSpeed_;

	// 移動させる
	worldTransform_->translation_ += velocity;
}