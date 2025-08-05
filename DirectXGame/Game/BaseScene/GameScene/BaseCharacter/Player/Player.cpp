#include "Player.h"
#include "../../GameScene.h"
#include "../BaseEnemy/BaseEnemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const Vector3& position, int32_t hp)
{
	// 基底クラスの初期化
	BaseCharacter::Initialize(engine, camera3d, modelHandleStore,position, hp);

	/*----------
	    本体
	----------*/

	// ワールドトランスフォームの生成と初期化
	bodyWorldTransform_ = std::make_unique<WorldTransform>();
	bodyWorldTransform_->Initialize();
	bodyWorldTransform_->SetParent(worldTransform_.get());

	bodyModelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kPlayer)[0];

	// ポイントライトの生成と初期化
	bodyPointLight_ = std::make_unique<PointLight>();
	bodyPointLight_->Initialize();
	bodyPointLight_->radius_ = 24.0f;

	// 浮遊ギミックの初期化
	gimmickFloating_ = std::make_unique<GimmickFloating>();
	gimmickFloating_->Initialize(bodyWorldTransform_.get(), 0.05f);


	/*-------------
	    サウンド
	-------------*/

	// 小発射音
	minShotSoundHandle_ = engine_->LoadSound("./Resources/Sounds/Se/player/bullet/shot.mp3");

	// 大発射音
	bigShotSoundHandle_ = engine_->LoadSound("./Resources/Sounds/Se/player/bullet/bigShot.mp3");

	// ダメージ音
	soundHandleDamage1_ = engine_->LoadSound("./Resources/Sounds/Se/player/damage/damage1.mp3");
	soundHandleDamage2_ = engine_->LoadSound("./Resources/Sounds/Se/player/damage/damage2.mp3");

	// 時計の音
	shClock_ = engine_->LoadSound("./Resources/Sounds/Se/player/attack/clock.mp3");
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{

	// 入力操作
	Input();

	// 浮遊ギミックの更新
	gimmickFloating_->Update();

	// ギミック : 傾き : 更新処理
	GimmickTiltUpdate();

	// ギミック : 発射 : 更新処理
	GimmickShotUpdate();

	// 基底クラスの更新処理
	BaseCharacter::Update();

	// 中心の更新
	worldTransform_->UpdateWorldMatrix();

	// 本体の更新
	bodyWorldTransform_->UpdateWorldMatrix();

	// 本体の位置にポイントライトを置く
	bodyPointLight_->position_ = GetBodyWorldPosition();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// ポイントライトを設置する
	engine_->SetPointLight(bodyPointLight_.get());

	// 本体を描画する
	engine_->DrawModel(bodyWorldTransform_.get(), camera3d_, bodyModelHandle_, Vector4(0.0f, 0.0f, 0.0f, 1.0f), true);
}


/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 Player::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}

/// <summary>
/// 本体のワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 Player::GetBodyWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = bodyWorldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = bodyWorldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = bodyWorldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}


/// <summary>
/// 衝突判定応答
/// </summary>
/// <param name="enemy"></param>
void Player::OnCollision(const BaseEnemy* enemy)
{
	// ダメージが入る
	hp_ -= enemy->GetPower();
	hp_ = std::max(hp_, 0);

	// カメラをシェイクする
	gameScene_->CameraShake(0.5f, 1.5f);

	// ダメージ音を鳴らす
	if (rand() % 2 == 0)
	{
		engine_->PlaySoundData(soundHandleDamage1_, 0.5f);
	}
	else
	{
		engine_->PlaySoundData(soundHandleDamage2_, 0.5f);
	}
}

/// <summary>
/// 衝突判定応答
/// </summary>
/// <param name="bullet"></param>
void Player::OnCollision(const BaseEnemyBullet* enemyBullet)
{
	// ダメージが入る
	hp_ -= enemyBullet->GetPower();
	hp_ = std::max(hp_, 0);

	// カメラをシェイクする
	gameScene_->CameraShake(0.5f, 1.5f);

	// ダメージ音を鳴らす
	if (rand() % 2 == 0)
	{
		engine_->PlaySoundData(soundHandleDamage1_, 0.5f);
	} 
	else
	{
		engine_->PlaySoundData(soundHandleDamage2_, 0.5f);
	}
}

/// <summary>
/// 当たり判定用のAABBを取得する
/// </summary>
/// <returns></returns>
AABB Player::GetCollisionAABB()const
{
	// AABB
	AABB aabb;

	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_->scale_, worldTransform_->rotation_, worldTransform_->translation_) *
		MakeAffineMatrix(bodyWorldTransform_->scale_, bodyWorldTransform_->rotation_, bodyWorldTransform_->translation_);

	aabb.max = hitSize_ + Vector3(worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2]);
	aabb.min = (-1.0f * hitSize_) + Vector3(worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2]);

	return aabb;
}



/*----------
    操作
----------*/

/// <summary>
/// 入力操作
/// </summary>
void Player::Input()
{
	// 移動操作
	Move();

	// 弾の発射操作
	BulletShot();

	// 時間操作
	OperationTimer();
}


/*   移動操作   */

/// <summary>
/// 移動操作
/// </summary>
void Player::Move()
{
	// 機体の傾きを初期化する
	gimmickTilt_ = kStraight;

	// ゲームパッドが有効な時
	if (engine_->IsGamepadEnable(0))
	{
		MoveGamepad();
	}
	else
	{
		// 無効な時
		MoveKeyboard();
	}
}

/// <summary>
/// ゲームパッドでの移動操作
/// </summary>
void Player::MoveGamepad()
{
	// デッドゾーン
	const float deadZone = 0.2f;

	// ゲームパッドの移動量
	Vector3 move = Vector3(engine_->GetGamepadLeftStick(0).x, engine_->GetGamepadLeftStick(0).y, 0.0f);

	// スティックの距離
	float stickLength = Length(move);

	// デッドゾーンを越えたら移動できる
	if (stickLength < deadZone)
		return;

	
	// 左右の移動量により機体を傾ける
	if (move.x > 0.7f)
	{
		gimmickTilt_ = kRight;
	}
	else if(move.x < -0.7f)
	{
		gimmickTilt_ = kLeft;
	}

	// 速度 と スティックの距離 を反映させて移動させる
	worldTransform_->translation_ += move * (speed * stickLength);

	// 移動範囲を越えたら傾かなくなる
	if (worldTransform_->translation_.x >= moveRange_.x || worldTransform_->translation_.x <= -moveRange_.x)
	{
		gimmickTilt_ = kStraight;
	}

	// 移動範囲を越えないようにする
	worldTransform_->translation_.x = std::min(worldTransform_->translation_.x, moveRange_.x);
	worldTransform_->translation_.x = std::max(worldTransform_->translation_.x, -moveRange_.x);
	worldTransform_->translation_.y = std::min(worldTransform_->translation_.y, moveRange_.y);
	worldTransform_->translation_.y = std::max(worldTransform_->translation_.y, -moveRange_.y);
}

/// <summary>
/// キーボードでの移動操作
/// </summary>
void Player::MoveKeyboard()
{

}



/*   発射操作   */

/// <summary>
/// 弾の発射操作
/// </summary>
void Player::BulletShot()
{
	// タイマーを進める
	shotTimer_ += 1.0f / 60.0f;
	shotTimer_ = std::min(shotTimer_, kBigShotTime);

	// 発射する時間を超えるまで処理しない
	if (shotTimer_ < kShotTime)
		return;


	// ゲームパッドが有効な時
	if (engine_->IsGamepadEnable(0))
	{
		BulletShotGamepad();
	}
	else
	{
		// 無効な時
		BulletShotKeyboard();
	}
}

/// <summary>
/// ゲームパッドでの弾の発射操作
/// </summary>
void Player::BulletShotGamepad()
{
	// Aボタンで弾を発射する
	if (engine_->GetGamepadButtonPress(0,XINPUT_GAMEPAD_A))
	{
		// プレイヤーの向きを取得する
		Vector3 direction = Normalize(Transform(Vector3(0.0f, 0.0f, 1.0f), MakeRotateMatrix(worldTransform_->rotation_)));

		// 大発射
		if (shotTimer_ >= kBigShotTime)
		{
			// 弾の生成と初期化
			std::unique_ptr<PlayerBulletStrong> newBullet = std::make_unique<PlayerBulletStrong>();
			newBullet->Initialize(engine_, camera3d_, modelHandleStore_, worldTransform_->translation_);
			newBullet->SetDirection(direction);

			// リストに登録する
			gameScene_->PlayerBulletShot(std::move(newBullet));

			// カメラをシェイクする
			gameScene_->CameraShake(0.5f, 1.0f);

			// タイマーを初期化する
			shotTimer_ = 0.0f;

			// 発射音を鳴らす
			engine_->PlaySoundData(bigShotSoundHandle_, 0.6f);
		}
		else
		{
			// 小発射

			// 弾の生成と初期化
			std::unique_ptr<PlayerBulletWeek> newBullet = std::make_unique<PlayerBulletWeek>();
			newBullet->Initialize(engine_, camera3d_, modelHandleStore_, worldTransform_->translation_);
			newBullet->SetDirection(direction);

			// リストに登録する
			gameScene_->PlayerBulletShot(std::move(newBullet));

			// タイマーを初期化する
			shotTimer_ = 1.0f;

			// 発射音を鳴らす
			engine_->PlaySoundData(minShotSoundHandle_, 0.3f);
		}

		// 発射ギミックの初期化
		GimmickShotInitialize();
	}
}

/// <summary>
/// キーボードでの弾の発射操作
/// </summary>
void Player::BulletShotKeyboard()
{

}



/*--------------
	時間操作
--------------*/

/// <summary>
/// 時間操作
/// </summary>
void Player::OperationTimer()
{
	// 最速になったら音声を止める
	if (phClockPicth_ >= 5.0f)
	{
		if(engine_->IsSoundPlay(phClock_))
			engine_->StopSound(phClock_);
	} 
	else
	{
		if (!engine_->IsSoundPlay(phClock_) || phClock_ == 0)
		{
			phClock_ = engine_->PlaySoundData(shClock_, 0.5f);
		}

		engine_->SetPitch(phClock_, phClockPicth_);
	}


	// 時間操作中
	if (isOperationTimer_)
	{
		// 時間操作の更新処理
		OperationTimerUpdate();
	}
	else
	{
		// クールタイムを進める
		if (operationTimerCooltimer_ < kMaxOperationTimerCooltimer)
		{
			operationTimerCooltimer_ += 1.0f / 60.0f;
			operationTimerCooltimer_ = std::min(operationTimerCooltimer_, kMaxOperationTimerCooltimer);

			if (operationTimerCooltimer_ <= 0.6f)
			{
				float t = operationTimerCooltimer_ / 0.5f;
				t = std::min(t, 1.0f);
				float easing = 1.0f - std::powf(1.0f - t, 3.0f);

				gameTimer_ = Lerp(0.1f, 1.0f, t);
				phClockPicth_ = Lerp(1.0f, 5.0f, t);
			}

			return;
		}

		// ゲームパッドが有効かどうか
		if (engine_->IsGamepadEnable(0))
		{
			OperationTimerGamepad();
		} 
		else
		{
			OperatoinTimerKeyboard();
		}
	}
}

/// <summary>
/// 時間操作の更新処理
/// </summary>
void Player::OperationTimerUpdate()
{
	// 遅延タイマーを進める
	slowTimer_ += 1.0f / 60.0f;

	// 最大値を超えないようにする
	slowTimer_ = std::min(slowTimer_, slowTime_);

	if (slowTimer_ <= 0.6f)
	{
		float t = slowTimer_ / 0.5f;
		t = std::min(t, 1.0f);
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		gameTimer_ = Lerp(1.0f, 0.1f, t);
		phClockPicth_ = Lerp(5.0f, 1.0f, t);
	}

	// 最大値についたら、時間操作を終了する
	if (slowTimer_ >= slowTime_)
	{
		isOperationTimer_ = false;

		// 時間操作クールタイムを開始する
		operationTimerCooltimer_ = 0.0f;

		return;
	}
}

/// <summary>
/// ゲームパッドでの時間操作
/// </summary>
void Player::OperationTimerGamepad()
{
	// 左スティック押し込みで時間操作する
	if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_LEFT_THUMB))
	{
		isOperationTimer_ = true;

		// 遅延タイマーを初期化する
		slowTimer_ = 0.0f;
	}
}

/// <summary>
/// キーボードでの時間操作
/// </summary>
void Player::OperatoinTimerKeyboard()
{

}



/*   傾き   */

/// <summary>
/// ギミック : 傾き : 更新処理
/// </summary>
void Player::GimmickTiltUpdate()
{
	// 状態に合わせて補間で傾ける
	bodyWorldTransform_->rotation_.z = Lerp(bodyWorldTransform_->rotation_.z, kGimmickTiltGoalRadian[gimmickTilt_], 0.1f);
}



/*   発射   */

/// <summary>
/// ギミック : 発射 : 初期化
/// </summary>
void Player::GimmickShotInitialize()
{
	// 発射パラメータ
	shotParameter_ = 0.0f;

	// 移動する
	bodyWorldTransform_->translation_.z = shotMove_.z;
}

/// <summary>
/// ギミック : 発射 : 更新処理
/// </summary>
void Player::GimmickShotUpdate()
{
	// パラメータを越えたら処理しない
	if (shotParameter_ >= kShotParameterMax)
		return;

	// パラメータを進める
	shotParameter_ += kShotParameterVelocity;
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);

	// 補間を求める
	float t = shotParameter_ / kShotParameterMax;
	float easing = t;

	// 徐々に初期位置に戻る
	bodyWorldTransform_->translation_.z = (1.0f - easing) * shotMove_.z + easing * shotGoal_.z;
}