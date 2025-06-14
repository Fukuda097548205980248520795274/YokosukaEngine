#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void Enemy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight , const Vector3& position)
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
	worldTransform_->rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_->UpdateWorldMatrix();

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");


	// 速度を設定する
	velocity_ = { -kWalkSpeed,0.0f,0.0f };

	// 歩きモーションの時間
	walkMotionTimer_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update()
{
	// 歩きモーションタイマーを加算する
	walkMotionTimer_ += 1.0f / 60.0f;
	if (walkMotionTimer_ >= kWalkMotionTime)
		walkMotionTimer_ = 0.0f;

	// サインカーブで回転させる
	float param = std::sin((2.0f * std::numbers::pi_v<float>) * (walkMotionTimer_ / kWalkMotionTime));
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_->rotation_.x = (degree / 180.0f) * std::numbers::pi_v<float>;


	// 移動させる
	worldTransform_->translation_ += velocity_;

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
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 1.0f , 0.0f , 0.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());
}

/// <summary>
/// ワールド座標を取得する
/// </summary>
/// <returns></returns>
Vector3 Enemy::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;
	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];
	return worldPosition;
}


/// <summary>
/// AABBを取得する
/// </summary>
/// <returns></returns>
AABB Enemy::GetAABB() const
{
	// ワールド座標
	Vector3 worldPosition = GetWorldPosition();

	AABB aabb;
	aabb.min = worldPosition - Vector3(1.0f, 1.0f, 1.0f);
	aabb.max = worldPosition + Vector3(1.0f, 1.0f, 1.0f);

	return aabb;
}

/// <summary>
/// 衝突応答処理
/// </summary>
/// <param name="player"></param>
void Enemy::OnCollision(const Player* player)
{
	(void)player;
}