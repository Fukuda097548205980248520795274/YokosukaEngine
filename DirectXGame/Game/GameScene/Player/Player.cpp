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

	// 行動 : 浮遊 : 初期化
	BehaviorFloatInitialize();
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	// 入力操作
	Input();

	// 行動 : 浮遊 : 更新処理
	BehaviorFloatUpdate();


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

}



/// <summary>
/// 行動 : 浮き : 初期化
/// </summary>
void Player::BehaviorFloatInitialize()
{
	floatParameter_ = 0.0f;
}

/// <summary>
/// 行動 : 浮き : 更新処理
/// </summary>
void Player::BehaviorFloatUpdate()
{
	// パラメータを進める
	floatParameter_ += kFloatPrameterVelocity;

	// 上限を越えたら初期化する
	floatParameter_ = std::fmod(floatParameter_, kMaxFloatParameter);

	// ふわふわさせる
	bodyWorldTransform_->translation_.y = std::sin(floatParameter_) * kFloatAmplitude;
}