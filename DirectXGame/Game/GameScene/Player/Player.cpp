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
	// 最も近い惑星を探す
	SarchNearPlanet();

	// 入力操作
	Input();

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
	sphere.center = GetWorldPosition() + fallUpVelocity_ * kDeltaTime;
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

	// ジャンプの値を初期化する
	speed_ = 0.0f;
	fallUpSpeed = 0.0f;
	fallUpVelocity_ = Vector3(0.0f, 0.0f, 0.0f);

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


	// 惑星の最近接点を求める
	Vector3 toPlanet = gravitationalField->GetPlanetInstance()->GetWorldPosition() - GetWorldPosition();
	float length = Length(toPlanet) - gravitationalField->GetPlanetInstance()->GetRadius();

	// 最も近い惑星リストに追加する
	nearPlanets_.emplace_back(std::make_pair(length, gravitationalField->GetPlanetInstance()));
}

/// <summary>
/// 入力操作
/// </summary>
void Player::Input()
{
	// 着地しているとき
	if (isGround_)
	{
		// 落下ベクトルを求める
		fallUpVelocity_ = Normalize(planetPosition_ - GetWorldPosition()) * kFallSpeed;

		// プレイヤーの位置を探す
		Vector3 normalizeToPlayer = Normalize(GetWorldPosition() - planetPosition_);

		theta_ = std::acos(normalizeToPlayer.x);
		if (worldTransform_->translation_.y <= planetPosition_.y)
			theta_ *= -1.0f;

		// 移動操作
		MoveInput();

		// 角速度を加算する
		theta_ += anglerTheta_;


		// 球面座標系で移動する
		worldTransform_->translation_ = planetPosition_ + SphericalCoordinate(toPlanetLength_, theta_, 0.0f);


		if (engine_->GetKeyTrigger(DIK_SPACE))
		{
			// ジャンプの初速を与える
			fallUpSpeed = kJumpStartSpeed;
			fallUpVelocity_ = (-1.0f * toGravity_) * fallUpSpeed;
		}
	} 
	else
	{
		// 着地していないとき

		// 重力場の中にいるとき
		if (isGravityPull_)
		{
			// 上昇加工する
			speed_ += fallUpSpeed * kDeltaTime;

			// 重力場の中心に落下する
			fallUpVelocity_ = -1.0f * (toGravity_ * speed_);

			// 落下する
			fallUpSpeed -= kFallSpeed * kDeltaTime;


			// プレイヤーの位置を探す
			Vector3 normalizeToPlayer = Normalize(GetWorldPosition() - planetPosition_);
			theta_ = std::acos(normalizeToPlayer.x);
			if (worldTransform_->translation_.y <= planetPosition_.y)
				theta_ *= -1.0f;


			// 移動操作
			MoveInput();

			// 角速度を加算する
			theta_ += anglerTheta_;


			// 球面座標系で移動する
			worldTransform_->translation_ = planetPosition_ + SphericalCoordinate(toPlanetLength_, theta_, 0.0f) + fallUpVelocity_;
		} else
		{
			// 重力場の外では無効になる
			fallUpVelocity_ = { 0.0f , 0.0f , 0.0f };
		}
	}
}

/// <summary>
/// 最も近い惑星を探す
/// </summary>
void Player::SarchNearPlanet()
{
	// リストに惑星があるかどうか
	if (!nearPlanets_.empty())
	{
		// 距離で昇順にソート
		nearPlanets_.sort();

		if (ridePlanet_ != nearPlanets_.front().second)
		{
			ridePlanet_ = nearPlanets_.front().second;

			fallUpSpeed *= -1.0f;
		}

		// 最も近い惑星の方向ベクトルを取得する
		toGravity_ = Normalize(nearPlanets_.front().second->GetWorldPosition() - GetWorldPosition());

		// 最も近い惑星の位置を取得する
		planetPosition_ = nearPlanets_.front().second->GetWorldPosition();
	}

	// リストをクリアする
	nearPlanets_.clear();
}

/// <summary>
/// 移動操作
/// </summary>
void Player::MoveInput()
{
	// 移動するためのキーを押していないときは処理しない
	if (!engine_->GetKeyPress(DIK_D) && !engine_->GetKeyPress(DIK_A) &&
		!engine_->GetKeyPress(DIK_W) && !engine_->GetKeyPress(DIK_S))
	{
		anglerTheta_ = 0.0f;
		isMove_ = false;
		return;
	}

	// 移動しているときは処理しない
	if (isMove_)
		return;

	// Dキーで右移動
	if (engine_->GetKeyPress(DIK_D))
	{
		// 星の裏側では反対方向に進む
		if (worldTransform_->translation_.y > planetPosition_.y)
		{
			anglerTheta_ = -kAnglerTheta;
		}
		else
		{
			anglerTheta_ = kAnglerTheta;
		}
	} 
	else if (engine_->GetKeyPress(DIK_A))
	{
		// Aキーで左移動

		// 星の裏側では反対方向に進む
		if (worldTransform_->translation_.y > planetPosition_.y)
		{
			anglerTheta_ = kAnglerTheta;
		} 
		else
		{
			anglerTheta_ = -kAnglerTheta;
		}
	}
	else if (engine_->GetKeyPress(DIK_W))
	{
		// 星の裏側では反対方向に進む
		if (worldTransform_->translation_.x > planetPosition_.x)
		{
			anglerTheta_ = kAnglerTheta;
		} 
		else
		{
			anglerTheta_ = -kAnglerTheta;
		}
	} 
	else if (engine_->GetKeyPress(DIK_S))
	{
		// Aキーで左移動

		// 星の裏側では反対方向に進む
		if (worldTransform_->translation_.x > planetPosition_.x)
		{
			anglerTheta_ = -kAnglerTheta;
		} else
		{
			anglerTheta_ = kAnglerTheta;
		}
	}

	// 移動する
	isMove_ = true;
}