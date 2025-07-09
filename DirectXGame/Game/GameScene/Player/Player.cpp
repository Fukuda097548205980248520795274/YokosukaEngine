#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;


	/*---------
	    中心
	----------*/

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();


	/*----------
	    本体
	----------*/

	// ワールドトランスフォームの生成と初期化
	bodyWorldTransform_ = std::make_unique<WorldTransform>();
	bodyWorldTransform_->Initialize();
	bodyWorldTransform_->SetParent(worldTransform_.get());

	// UVトランスフォームの生成と初期化
	bodyUvTransform_ = std::make_unique<UvTransform>();
	bodyUvTransform_->Initialize();

	// モデルを読み込む
	bodyModelHandle_ = engine_->LoadModelData("./Resources/Models/Player", "Player.obj");

	// ギミック : 浮遊 : 初期化
	GimmickFloatInitialize();
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

	UpdateMoveBehavior();


	// 中心の更新
	worldTransform_->UpdateWorldMatrix();

	// 本体の更新
	bodyWorldTransform_->UpdateWorldMatrix();
	bodyUvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// 本体を描画する
	engine_->DrawModel(bodyWorldTransform_.get(), bodyUvTransform_.get(), camera3d_, bodyModelHandle_, Vector4(0.0f, 0.0f, 0.0f, 1.0f), true);
}

/// <summary>
/// 入力操作
/// </summary>
void Player::Input()
{
	// 移動操作
	Move();
}

/// <summary>
/// 移動操作
/// </summary>
void Player::Move()
{
	// ゲームパッドが有効な時
	if (engine_->IsGamepadEnable(0))
	{
		MoveGamepad();
	}
	else
	{
		// 無効な時
		MoveKeybord();
	}
}

/// <summary>
/// ゲームパッドでの移動操作
/// </summary>
void Player::MoveGamepad()
{
	// デッドゾーン
	const float deadZone = 0.7f;

	// ゲームパッドの移動量
	Vector3 move = Vector3(engine_->GetGamepadLeftStick(0).x, engine_->GetGamepadLeftStick(0).y, 0.0f);


	moveBehavior_ = kStraight;

	// デッドゾーンを越えたら移動できる
	if (Length(move) < deadZone)
		return;

	
	if (move.x > 0.0f)
	{
		moveBehavior_ = kRight;
	}
	else if(move.x < 0.0f)
	{
		moveBehavior_ = kLeft;
	}

	// 速度を反映させて移動させる
	worldTransform_->translation_ += move * speed;
}

/// <summary>
/// キーボードでの移動操作
/// </summary>
void Player::MoveKeybord()
{

}


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




/// <summary>
/// 移動ビヘイビアの更新処理
/// </summary>
void Player::UpdateMoveBehavior()
{
	bodyWorldTransform_->rotation_.z = Lerp(bodyWorldTransform_->rotation_.z, kMoveBehaviorGoalRadian[moveBehavior_], 0.1f);
}