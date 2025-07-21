#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Enemy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラス初期化
	BaseCharacter::Initialize(engine, camera3d, position);


	// 胴体
	models_[kBody].worldTransform = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform->Initialize();
	models_[kBody].worldTransform->SetParent(worldTransform_.get());
	models_[kBody].worldTransform->translation_ = modelsStartPosition[kBody];
	models_[kBody].modelHandle = engine_->LoadModelData("./Resources/Models/Enemy/body", "body.obj");
	models_[kBody].color = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	// わっか
	models_[kRing].worldTransform = std::make_unique<WorldTransform>();
	models_[kRing].worldTransform->Initialize();
	models_[kRing].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kRing].worldTransform->translation_ = modelsStartPosition[kRing];
	models_[kRing].modelHandle = engine_->LoadModelData("./Resources/Models/Enemy/ring", "ring.obj");
	models_[kRing].color = Vector4(0.3f, 0.0f, 0.0f, 1.0f);

	// 刃
	models_[kBlade].worldTransform = std::make_unique<WorldTransform>();
	models_[kBlade].worldTransform->Initialize();
	models_[kBlade].worldTransform->SetParent(models_[kRing].worldTransform.get());
	models_[kBlade].worldTransform->translation_ = modelsStartPosition[kBlade];
	models_[kBlade].modelHandle = engine_->LoadModelData("./Resources/Models/Enemy/blade", "blade.obj");
	models_[kBlade].color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);

	for (uint32_t i = 0; i < kNumModels; ++i)
	{
		// UVトランスフォームの生成と初期化
		models_[i].uvTransform = std::make_unique<UvTransform>();
		models_[i].uvTransform->Initialize();

		// 影の生成と初期化
		models_[i].planeProjectionShadow = std::make_unique<PlaneProjectionShadow>();
		models_[i].planeProjectionShadow->Initialize(engine_, camera3d_, models_[i].modelHandle, models_[i].worldTransform.get());
	}

	// 回転移動ギミックの初期化
	InitializeRotateMoveGimmick();

	// 浮遊ギミックの初期化
	GimmickFloatingInitialize();

	// 回転ギミックの初期化
	GimmickRotateInitialize();
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update()
{
	// 回転移動ギミックの更新処理
	UpdateRotateMoveGimmick();

	// 浮遊ギミックの更新処理
	GimmickFloatingUpdate();

	// 回転ギミックの更新処理
	GimmickRotateUpdate();

	// 基底クラス更新処理
	BaseCharacter::Update();

	// モデルの更新処理
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		models_[i].worldTransform->UpdateWorldMatrix();
		models_[i].uvTransform->UpdateWorldMatrix();
		models_[i].planeProjectionShadow->Update();
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
		models_[i].planeProjectionShadow->Draw();
		engine_->DrawModel(models_[i].worldTransform.get(), models_[i].uvTransform.get(), camera3d_,
			models_[i].modelHandle, models_[i].color, false);
	}
}

/// <summary>
/// 中心座標のGetter
/// </summary>
/// <returns></returns>
Vector3 Enemy::GetCenterPosition() const
{
	// 見た目上の中心オフセット
	Vector3 offset = { 0.0f, 0.3f, 0.0f };

	// ワールド座標に変換する
	Vector3 worldPosition = Transform(offset, worldTransform_->worldMatrix_);
	return worldPosition;
}

/// <summary>
/// 衝突判定応答
/// </summary>
void Enemy::OnCollision()
{

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


/// <summary>
/// ギミック : 浮遊 : 初期化
/// </summary>
void Enemy::GimmickFloatingInitialize()
{
	// 浮遊ギミックパラメータ
	floatingParamter_ = 0.0f;
}

/// <summary>
/// ギミック : 浮遊 : 更新処理
/// </summary>
void Enemy::GimmickFloatingUpdate()
{
	// 1フレームのパラメータ
	const float step = 2.0f * std::numbers::pi_v<float> / (floatingTime_ * 60.0f);

	// パラメータを加算する
	floatingParamter_ += step;
	floatingParamter_ = std::fmod(floatingParamter_, 2.0f * std::numbers::pi_v<float>);

	models_[kRing].worldTransform->translation_.y = modelsStartPosition[kRing].y + std::sin(floatingParamter_) * floatingAmplitude_;
}


/// <summary>
/// ギミック : 回転 : 初期化
/// </summary>
void Enemy::GimmickRotateInitialize()
{
	// 回転ギミックのパラメータ
	rotateParameter_ = 0.0f;
}

/// <summary>
/// ギミック : 回転 : 更新処理
/// </summary>
void Enemy::GimmickRotateUpdate()
{
	// 1フレームのパラメータ
	const float step = 2.0f * std::numbers::pi_v<float> / (rotateTime_ * 60.0f);

	// パラメターを加算する
	rotateParameter_ += step;
	rotateParameter_ = std::fmod(rotateParameter_, 2.0f * std::numbers::pi_v<float>);

	models_[kBlade].worldTransform->rotation_.y = rotateParameter_;
}