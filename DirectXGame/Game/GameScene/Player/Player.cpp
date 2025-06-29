#include "Player.h"
#include "../Planet/Planet.h"
#include "../GravitationalField/GravitationalField.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
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
	worldTransform_->translation_ = position;
	worldTransform_->scale_ *= radius_;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	ImGui::Begin("Player");
	ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
	ImGui::End();

	// 移動速度
	Vector3 velocity = { 0.0f , 0.0f , 0.0f };

	// 着地しているとき
	if (isGround_)
	{
		// プレイヤーの位置を探す
		Vector3 normalizeToPlayer = Normalize(GetWorldPosition() - planetPosition_);
		anglerVelocity_ = std::acos(normalizeToPlayer.x);
		if (normalizeToPlayer.y <= 0.0f)
			anglerVelocity_ *= -1.0f;

		ImGui::Text("%f", anglerVelocity_);

		if (engine_->GetKeyPress(DIK_D))
		{
			anglerVelocity_ -= 0.01f;
		}
		else if (engine_->GetKeyPress(DIK_A))
		{
			anglerVelocity_ += 0.01f;
		}

		worldTransform_->translation_ = planetPosition_ + SphericalCoordinate(toPlanetLength_, anglerVelocity_, 0.0f);
	}
	else
	{
		// 着地していないとき

		// 重力場の中にいるとき
		if (isGravityPull_)
		{
			// 移動させる
			velocity = toGravity_ * kFallSpeed;
		}
	}

	// 移動させる
	worldTransform_->translation_ += velocity;

	isGround_ = false;
	isGravityPull_ = false;

	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// 球を描画する
	engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_, textureHandle_, Vector4(1.0f, 0.0f, 0.0f, 1.0), true);
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 Player::GetWorldPosition()
{
	// ワールド座標
	Vector3 worldPosition;
	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];
	return worldPosition;
}

/// <summary>
/// 当たり判定の球のGetter
/// </summary>
/// <returns></returns>
Sphere Player::GetCollisionSphere()
{
	Sphere sphere;
	sphere.center = GetWorldPosition() + (toGravity_ * kFallSpeed);
	sphere.radius = radius_;
	return sphere;
}

/// <summary>
/// 衝突判定応答
/// </summary>
/// <param name="planet"></param>
void Player::OnCollision(const Planet* planet)
{
	// 着地する
	isGround_ = true;

	// 惑星とプレイヤーの距離を取得する
	planetPosition_ = planet->GetWorldPosition();
	toPlanetLength_ = planet->GetRadius() + radius_;
}

/// <summary>
/// 衝突判定応答
/// </summary>
void Player::OnCollision(const GravitationalField* gravitationalField)
{
	// 重力に引っ張られる
	isGravityPull_ = true;

	// 重力場の中心へのベクトルを取得する
	toGravity_ = Normalize(gravitationalField->GetWorldPosition() - GetWorldPosition());
}