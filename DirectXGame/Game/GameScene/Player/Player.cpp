#include "Player.h"

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	// 弾
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
	bullets_.clear();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight)
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

	// 弾の更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	// 消滅した弾をリストから排除する
	bullets_.remove_if([](PlayerBullet* bullet)
		{
			if (bullet->IsFinished())
			{
				delete bullet;
				return true;
			}
			return false;
		}
	);


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
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 0.0f, 0.0f, 0.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());

	// 弾の描画
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw();
	}
}


/// <summary>
/// 弾を発射する
/// </summary>
void Player::BulletShot()
{
	// Aボタンで弾を発射する
	if (engine_->IsGamepadEnable(0))
	{
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_A))
		{
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity = Vector3{ 0.0f , 0.0f , kBulletSpeed };

			// プレイヤーの回転行列
			Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->rotation_);

			// 速度ベクトルと回転行列で座標変換
			velocity = TransformNormal(velocity, rotateMatrix);


			// 弾の生成と初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(engine_, camera3d_, directionalLight_, GetWorldPosition(), velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
	}
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