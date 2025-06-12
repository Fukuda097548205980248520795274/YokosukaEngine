#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, const DirectionalLight* directionalLight)
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
	worldTransform_->rotation_.y = -std::numbers::pi_v<float> / 2.0f;

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
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	// ゲームパッドが有効かどうか
	if (engine_->IsGamepadEnable(0))
	{
		// 加速度ベクトル
		Vector3 acceleration = { 0.0f , 0.0f , 0.0f };


		/*----------
		    移動
		----------*/

		// 操作中
		if (engine_->GetGamepadLeftStick(0).x != 0.0f)
		{
			// 右スティックを右で、右移動
			if (engine_->GetGamepadLeftStick(0).x >= 0.9f)
			{
				// 左移動中に右に移動すると速度減衰
				if (velocity_.x < 0.0f)
				{
					velocity_.x *= (1.0f - kAttenuation);
				}

				// 右に振り向く
				if (lrDirection_ != LRDirection::kRight)
				{
					lrDirection_ = LRDirection::kRight;

					// 旋回開始する
					turnFirstRotationY_ = worldTransform_->rotation_.y;
					turnTimer_ = 0.0f;
				}

				acceleration.x += kAddAcceleration;
			}
			else if (engine_->GetGamepadLeftStick(0).x <= -0.9f)
			{
				// 左スティックを左で、左移動

				// 右移動中に左に移動すると速度減衰
				if (velocity_.x > 0.0f)
				{
					velocity_.x *= (1.0f - kAttenuation);
				}

				// 左に振り向く
				if (lrDirection_ != LRDirection::kLeft)
				{
					lrDirection_ = LRDirection::kLeft;

					// 旋回開始する
					turnFirstRotationY_ = worldTransform_->rotation_.y;
					turnTimer_ = 0.0f;
				}

				acceleration.x -= kAddAcceleration;
			}
		}
		else
		{
			// 操作していないと、移動速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
		}

		// 速度ベクトルに加速度ベクトルを加算する
		velocity_ += acceleration;

		// 最大速度を越えないようにする
		velocity_.x = std::clamp(velocity_.x, -kMaxMoveSpeed, kMaxMoveSpeed);


		/*-------------
		    ジャンプ
		-------------*/

		// 地上に接地しているとき
		if (isGround_)
		{
			// Aボタンでジャンプする
			if (engine_->GetGamepadButtonTrigger(0,XINPUT_GAMEPAD_A))
			{
				// ジャンプ初速を加算する
				velocity_ += Vector3(0.0f, kJumpStartAcceleration, 0.0f);

				// 接地しなくなる
				isGround_ = false;
			}
		} 
		else
		{
			// 地上に接地していないとき

			// 落下速度
			velocity_ += Vector3(0.0f, -kGravityAcceleration, 0.0f);

			// 落下速度制限
			velocity_.y = std::max(velocity_.y, -kMaxFallSpeed);


			// 着地フラグ
			bool isLanding = false;

			// 地面との当たり判定
			// 落下中に地面についたら着地する
			if (velocity_.y < 0.0f)
			{
				if (worldTransform_->translation_.y <= 2.0f)
				{
					isLanding = true;
				}
			}


			// 着地したら、設置したことになる
			if (isLanding)
			{
				// めり込み排斥
				worldTransform_->translation_.y = 2.0f;

				// 摩擦で横方向速度が減衰する
				velocity_.x *= (1.0f - kAttenuation);

				// 下方向速度をリセット
				velocity_.y = 0.0f;

				// 接地状態に移行
				isGround_ = true;
			}
		}
	}


	// 速度ベクトルで移動させる
	worldTransform_->translation_ += velocity_;

	// 旋回制御
	Turn();


	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());
}


/// <summary>
/// 旋回制御
/// </summary>
void Player::Turn()
{
	if (turnTimer_ <= kTimeTurn)
	{
		// 時間の割合を求める
		float t = turnTimer_ / kTimeTurn;

		// 時間をカウントする
		turnTimer_ += 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] =
		{
			-std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> / 2.0f,
		};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 角度を設定する
		worldTransform_->rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, t);
	}
}