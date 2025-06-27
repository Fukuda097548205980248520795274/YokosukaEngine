#include "Player.h"
#include "../GameScene.h"
#include "../LockOn/LockOn.h"

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
/// <param name="directionalLight"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d, const DirectionalLight* directionalLight, const Vector3& posision)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(camera2d);
	assert(directionalLight);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	camera2d_ = camera2d;
	directionalLight_ = directionalLight;

	// 衝突属性と衝突マスクを設定する
	SetCollisionAttribute(0b1);
	SetCollisionMask(0b1 << 1);


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = posision;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Player", "Player.obj");

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;


	// ワールドトランスフォームの生成と初期化
	worldTransform3DReticle_ = std::make_unique<WorldTransform>();
	worldTransform3DReticle_->Initialize();
	worldTransform3DReticle_->translation_.z = 50.0f;

	worldTransform2DReticle_ = std::make_unique<WorldTransform>();
	worldTransform2DReticle_->Initialize();
	worldTransform2DReticle_->scale_ = Vector3(32.0f, 32.0f, 1.0);

	// UVトランスフォームの生成と初期化
	uvTransform2DReticle_ = std::make_unique<UvTransform>();
	uvTransform2DReticle_->Initialize();

	// モデルを読み込む
	modelHandle3DReticle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");

	// テクスチャを読み込む
	textureHandle2DReticle_ = engine_->LoadTexture("./Resources/Textures/reticle.png");
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{

	/*---------
	    移動
	---------*/

	// プレイヤーの移動ベクトル
	Vector3 move = { 0.0f , 0.0f , 0.0f };

	// プレイヤー移動の速さ
	const float kMoveSpeed = 0.4f;

	// 左スティックを傾けた方向に移動する
	if (engine_->IsGamepadEnable(0))
	{
		move.x = kMoveSpeed * engine_->GetGamepadLeftStick(0).x;
		move.y = kMoveSpeed * engine_->GetGamepadLeftStick(0).y;
	}

	// 座標移動する
	worldTransform_->translation_ += move;

	
	// 移動限界座標
	const float kMoveLimitX = 18.0f;
	const float kMoveLimitY = 10.0f;

	// 範囲を越えないようにする
	worldTransform_->translation_.x = std::clamp(worldTransform_->translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_->translation_.y = std::clamp(worldTransform_->translation_.y, -kMoveLimitY, kMoveLimitY);


	/*----------
	    旋回
	----------*/

	// 旋回のY軸回転速度
	float kTurnYSpeed = 0.02f;

	// RB / LBで旋回する
	if (engine_->GetGamepadButtonPress(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		worldTransform_->rotation_.y -= kTurnYSpeed;
	}
	else if (engine_->GetGamepadButtonPress(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		worldTransform_->rotation_.y += kTurnYSpeed;
	}


	// 弾を発射する
	BulletShot();


	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();



	/*----------------
	    3Dレティクル
	----------------*/

	// 左スティックで3Dレティクルを操作する
	if (engine_->IsGamepadEnable(0))
	{
		worldTransform3DReticle_->translation_.x += engine_->GetGamepadRightStick(0).x;
		worldTransform3DReticle_->translation_.y += engine_->GetGamepadRightStick(0).y;
	}

	worldTransform3DReticle_->translation_.x = std::clamp(worldTransform3DReticle_->translation_.x, -30.0f, 30.0f);
	worldTransform3DReticle_->translation_.y = std::clamp(worldTransform3DReticle_->translation_.y, -20.0f, 20.0f);

	// トランスフォームを更新する
	worldTransform3DReticle_->UpdateWorldMatrix();


	Vector3 reticlePosition = GetWorldPosision3DReticle();

	// ビューポート変換行列
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, camera2d_->screenWidth_, camera2d_->screenHeight_, 0.0f, 100.0f);

	// ビュー行列　正射影行列　ビューポート変換行列を合成する
	Matrix4x4 viewProjectionViewportMatrix = 
		camera3d_->viewMatrix_ * camera3d_->projectionMatrix_ * viewportMatrix;

	// ワールドからスクリーンに変換する
	reticlePosition = Transform(reticlePosition, viewProjectionViewportMatrix);
	worldTransform2DReticle_->translation_.x = reticlePosition.x;
	worldTransform2DReticle_->translation_.y = reticlePosition.y;

	// トランスフォームを更新する
	worldTransform2DReticle_->UpdateWorldMatrix();
	uvTransform2DReticle_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 0.0f, 0.0f, 0.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());

	// 2Dレティクルを描画する
	engine_->DrawModel(worldTransform3DReticle_.get(), uvTransform2DReticle_.get(), camera3d_, modelHandle3DReticle_, Vector4(0.0f, 0.0f, 0.0f, 0.5f));
}


/// <summary>
/// 弾を発射する
/// </summary>
void Player::BulletShot()
{
	// Aボタンで弾を発射する
	if (engine_->IsGamepadEnable(0))
	{
		if (engine_->GetGamepadRightTrigger(0) > 0.5f)
		{
			if (lockOn_->IsTarget())
			{
				// 弾の速度
				const float kBulletSpeed = 2.0f;

				// 3dレティクル方向のベクトル（正規化）
				Vector3 velocity = Normalize(lockOn_->GetTargetInstance()->GetWorldPosition() - GetWorldPosition()) * kBulletSpeed;


				// 弾の生成と初期化
				PlayerBullet* newBullet = new PlayerBullet();
				newBullet->Initialize(engine_, camera3d_, directionalLight_, GetWorldPosition(), velocity);

				// 弾を登録する
				gameScene_->PushPlayerBullet(newBullet);
			}
			else
			{
				// 弾の速度
				const float kBulletSpeed = 2.0f;

				// 3dレティクル方向のベクトル（正規化）
				Vector3 velocity = Normalize(GetWorldPosision3DReticle() - GetWorldPosition()) * kBulletSpeed;


				// 弾の生成と初期化
				PlayerBullet* newBullet = new PlayerBullet();
				newBullet->Initialize(engine_, camera3d_, directionalLight_, GetWorldPosition(), velocity);

				// 弾を登録する
				gameScene_->PushPlayerBullet(newBullet);
			}
		}
	}
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
/// 3Dレティクルのワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 Player::GetWorldPosision3DReticle() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform3DReticle_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform3DReticle_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform3DReticle_->worldMatrix_.m[3][2];

	return worldPosition;
}

/// <summary>
/// 衝突コールバック関数
/// </summary>
void Player::OnCollision()
{

}