#include "Player.h"
#include "../../GameScene.h"
#include "../BaseEnemy/BaseEnemy.h"

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, int32_t hp)
{
	// 基底クラスの初期化
	BaseCharacter::Initialize(engine, camera3d, modelHandleStore, hp);


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

	// ギミック : 浮遊 : 初期化
	GimmickFloatInitialize();


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
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	// 入力操作
	Input();

	// ギミック : 浮遊 : 更新処理
	GimmickFloatUpdate();

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



/*-------------
    ギミック
-------------*/

/*   浮き   */

/// <summary>
/// ギミック : 浮き : 初期化
/// </summary>
void Player::GimmickFloatInitialize()
{
	// 浮きパラメータ
	floatParameter_ = 0.0f;

	// 浮きパラメータ速度
	kFloatPrameterVelocity = 0.05f;

	// 浮き振れ幅
	kFloatAmplitude = 0.3f;
}

/// <summary>
/// ギミック : 浮き : 更新処理
/// </summary>
void Player::GimmickFloatUpdate()
{
	// パラメータを進める
	floatParameter_ += kFloatPrameterVelocity;

	// 上限を越えたら初期化する
	floatParameter_ = std::fmod(floatParameter_, kMaxFloatParameter);

	// ふわふわさせる
	bodyWorldTransform_->translation_.y = std::sin(floatParameter_) * kFloatAmplitude;
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