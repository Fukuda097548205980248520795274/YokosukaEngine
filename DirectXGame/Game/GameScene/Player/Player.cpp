#include "Player.h"
#include "../MapChipField/MapChipField.h"

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
	// 移動入力
	InputMove();

	// 当たり判定情報
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.velocity = velocity_;

	// マップとの当たり判定
	MapCollision(collisionMapInfo);

	CollisionMove(collisionMapInfo);

	CeilingCollision(collisionMapInfo);

	WallCollision(collisionMapInfo);

	GroundOnSwitch(collisionMapInfo);

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


/// <summary>
/// 移動入力
/// </summary>
void Player::InputMove()
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
			} else if (engine_->GetGamepadLeftStick(0).x <= -0.9f)
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
		} else
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
			if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_A))
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
		}
	}
}

/// <summary>
/// マップとの当たり判定
/// </summary>
/// <param name="info">マップとの当たり判定情報</param>
void Player::MapCollision(CollisionMapInfo& info)
{
	// 上方向の当たり判定
	MapCollisionTop(info);

	// 下方向の当たり判定
	MapCollisionBottom(info);

	// 右方向の当たり判定
	MapCollisionRight(info);

	// 左方向の当たり判定
	MapCollisionLeft(info);
}

/// <summary>
/// マップとの上方向当たり判定
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionTop(CollisionMapInfo& info)
{
	// 上移動していないときはスルー
	if (info.velocity.y <= 0.0f)
	{
		return;
	}

	// 移動前の4つの角度
	std::array<Vector3, kNumCorner> positionCurrent;

	// 移動後の四つの角度
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < kNumCorner; ++i)
	{
		positionCurrent[i] = CornerPosition(worldTransform_->translation_, static_cast<Corner>(i));
		positionNew[i] = CornerPosition(worldTransform_->translation_ + info.velocity, static_cast<Corner>(i));
	}


	// マップチップの種類
	MapChipType mapChipType;

	// 当たったかどうか
	bool isCollision = false;

	// マップチップ番号
	MapChipField::IndexSet indexSet;

	// 左上の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionCurrent[kLeftTop].x , positionNew[kLeftTop].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 右上の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionCurrent[kRightTop].x , positionNew[kRightTop].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 当たったとき
	if (isCollision)
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);

		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex , indexSet.yIndex);
		info.velocity.y = std::max(0.0f, rect.bottom - worldTransform_->translation_.y - 1.0f);

		// 天井に当たった子を記録する
		info.isCeilingCollision = true;
	}
}

/// <summary>
/// マップとの下方向当たり判定
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionBottom(CollisionMapInfo& info)
{
	// 落下していないときはスルー
	if (info.velocity.y >= 0.0f)
	{
		return;
	}

	// 移動前の4つの角度
	std::array<Vector3, kNumCorner> positionCurrent;

	// 移動後の四つの角度
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < kNumCorner; ++i)
	{
		positionCurrent[i] = CornerPosition(worldTransform_->translation_, static_cast<Corner>(i));
		positionNew[i] = CornerPosition(worldTransform_->translation_ + info.velocity, static_cast<Corner>(i));
	}


	// マップチップの種類
	MapChipType mapChipType;

	// 当たったかどうか
	bool isCollision = false;

	// マップチップ番号
	MapChipField::IndexSet indexSet;

	// 左下の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionCurrent[kLeftBottom].x , positionNew[kLeftBottom].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 右下の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionCurrent[kRightBottom].x , positionNew[kLeftBottom].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 当たったとき
	if (isCollision)
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);

		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::min(0.0f, rect.top - worldTransform_->translation_.y + 1.0f);

		// 天井に当たった子を記録する
		info.isLanding = true;
	}
}

/// <summary>
/// マップとの右方向当たり判定
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionRight(CollisionMapInfo& info)
{
	// 上移動していないときはスルー
	if (info.velocity.x <= 0.0f)
	{
		return;
	}

	// 移動前の4つの角度
	std::array<Vector3, kNumCorner> positionCurrent;

	// 移動後の四つの角度
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < kNumCorner; ++i)
	{
		positionCurrent[i] = CornerPosition(worldTransform_->translation_, static_cast<Corner>(i));
		positionNew[i] = CornerPosition(worldTransform_->translation_ + info.velocity, static_cast<Corner>(i));
	}


	// マップチップの種類
	MapChipType mapChipType;

	// 当たったかどうか
	bool isCollision = false;

	// マップチップ番号
	MapChipField::IndexSet indexSet;

	// 右上の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionNew[kRightTop].x , positionCurrent[kRightTop].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 右下の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionNew[kRightBottom].x , positionCurrent[kRightBottom].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 当たったとき
	if (isCollision)
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);

		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::max(0.0f, rect.left - worldTransform_->translation_.x - 1.0f);

		// 壁に当たったことを記録する
		info.isWallCollision = true;
	}
}

/// <summary>
/// マップとの左方向当たり判定
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionLeft(CollisionMapInfo& info)
{
	// 左移動していないときはスルー
	if (info.velocity.x >= 0.0f)
	{
		return;
	}

	// 移動前の4つの角度
	std::array<Vector3, kNumCorner> positionCurrent;

	// 移動後の四つの角度
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < kNumCorner; ++i)
	{
		positionCurrent[i] = CornerPosition(worldTransform_->translation_, static_cast<Corner>(i));
		positionNew[i] = CornerPosition(worldTransform_->translation_ + info.velocity, static_cast<Corner>(i));
	}


	// マップチップの種類
	MapChipType mapChipType;

	// 当たったかどうか
	bool isCollision = false;

	// マップチップ番号
	MapChipField::IndexSet indexSet;

	// 左上の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionNew[kLeftTop].x , positionCurrent[kLeftTop].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 左下の当たり判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition({ positionNew[kLeftBottom].x , positionCurrent[kLeftBottom].y , 0.0f });
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		isCollision = true;
	}

	// 当たったとき
	if (isCollision)
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);

		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::min(0.0f, rect.right - worldTransform_->translation_.x + 1.0f);

		// 壁に当たったことを記録する
		info.isWallCollision = true;
	}
}

/// <summary>
/// 角の座標を取得する
/// </summary>
/// <param name="center"></param>
/// <param name="corner"></param>
/// <returns></returns>
Vector3 Player::CornerPosition(const Vector3& center, Corner corner)
{
	Vector3 offsetTable[kNumCorner] =
	{
		{kCollisionSize, -kCollisionSize, 0.0f},
		{-kCollisionSize, -kCollisionSize, 0.0f},
		{kCollisionSize, kCollisionSize, 0.0f},
		{-kCollisionSize, kCollisionSize, 0.0f}
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

/// <summary>
/// 判定結果を反映して移動させる
/// </summary>
/// <param name="info"></param>
void Player::CollisionMove(const CollisionMapInfo& info)
{
	worldTransform_->translation_ += info.velocity;
}

/// <summary>
/// 天井に当たったときの判定処理
/// </summary>
/// <param name="info"></param>
void Player::CeilingCollision(const CollisionMapInfo& info)
{
	if (info.isCeilingCollision)
	{
		velocity_.y = 0.0f;
	}
}

/// <summary>
/// 壁に当たったときの判定処理
/// </summary>
/// <param name="info"></param>
void Player::WallCollision(const CollisionMapInfo& info)
{
	if (info.isWallCollision)
	{
		velocity_.x = 0.0f;
	}
}

/// <summary>
/// 接地状態の切り替え処理
/// </summary>
/// <param name="info"></param>
void Player::GroundOnSwitch(const CollisionMapInfo& info)
{
	// 接地状態
	if (isGround_)
	{
		// ジャンプ開始
		if (velocity_.y > 0.0f)
		{
			isGround_ = false;
		}
		else
		{
			// 移動前の4つの角度
			std::array<Vector3, kNumCorner> positionCurrent;

			// 吸着しないよう余白を開ける
			for (uint32_t i = 0; i < kNumCorner; ++i)
			{
				positionCurrent[i] = CornerPosition(worldTransform_->translation_, static_cast<Corner>(i));
			}

			// マップチップの種類
			MapChipType mapChipType;

			// 当たったかどうか
			bool isCollision = false;

			// マップチップ番号
			MapChipField::IndexSet indexSet;

			// 左上の当たり判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionCurrent[kLeftBottom] + Vector3(0.0f, -kAdsorptionMargin, 0.0f));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock)
			{
				isCollision = true;
			}

			// 右上の当たり判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionCurrent[kRightBottom] + Vector3(0.0f, -kAdsorptionMargin, 0.0f));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock)
			{
				isCollision = true;
			}

			// 当たっていないときは接地しない
			if (!isCollision)
			{
				isGround_ = false;
			}
		}
	}
	else
	{
		// 空中状態

		if (info.isLanding)
		{
			// 接地状態に切り替える
			isGround_ = true;

			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);

			// Y速度を0にする
			velocity_.y = 0.0f;
		}
	}
}