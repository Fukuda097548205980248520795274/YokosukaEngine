#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void Enemy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight,
	const Vector3& position)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(directionalLight);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	directionalLight_ = directionalLight;


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Enemy", "Enemy.obj");

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update()
{
	// 行動フェーズでの動き
	switch (phase_)
	{
	case Phase::kApproach:
		// 接近

		PhaseApproachUpdate();

		break;

	case Phase::kLeave:
		// 離脱

		PhaseLeaveUpdate();
		
		break;
	}

	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Enemy::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 1.0f, 0.0f, 0.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());
}


/// <summary>
	/// 接近フェーズの更新処理
	/// </summary>
void Enemy::PhaseApproachUpdate()
{
	// 移動速度
	const float kMoveSpeed = 0.1f;

	// 座標移動する
	worldTransform_->translation_.z -= kMoveSpeed;

	// 離脱フェーズに切り替える
	if (worldTransform_->translation_.z <= 0.0f)
	{
		phase_ = Phase::kLeave;
	}
}

/// <summary>
/// 離脱フェーズの更新処理
/// </summary>
void Enemy::PhaseLeaveUpdate()
{
	// 移動速度
	const float kMoveSpeed = 0.3f;

	// 速度ベクトル
	Vector3 velocity = { 0.0f , 1.0f , 0.0f };

	// 座標移動する
	worldTransform_->translation_ += kMoveSpeed * velocity;
}