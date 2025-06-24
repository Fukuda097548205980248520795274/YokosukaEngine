#include "PlayerBullet.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void PlayerBullet::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight,
	const Vector3& position, const Vector3& velocity)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(directionalLight);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	directionalLight_ = directionalLight;
	velocity_ = velocity;

	// 衝突属性と衝突マスクを設定する
	SetCollisionAttribute(0b1);
	SetCollisionMask(0b1 << 1);


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/PlayerBullet", "PlayerBullet.obj");

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
void PlayerBullet::Update()
{
	// 生存タイマーを進める
	lifeTimer_ += 1.0f / 60.0f;

	// 生存時間を越えたら消滅する
	if (lifeTimer_ >= kLifeTime)
	{
		isFinished_ = true;
	}


	// 座標移動させる
	worldTransform_->translation_ += velocity_;

	// 進む方向を向く
	worldTransform_->rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velocityXZ = Length(Vector3{ velocity_.x , 0.0f , velocity_.z });
	worldTransform_->rotation_.x = std::atan2(velocity_.y, velocityXZ);

	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerBullet::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 0.0f, 0.0f, 0.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 PlayerBullet::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}

/// <summary>
/// 衝突コールバック関数
/// </summary>
void PlayerBullet::OnCollision()
{
	// 終了する
	isFinished_ = true;
}