#include "DebugCamera.h"
#include "../../YokosukaEngine.h"

/// <summary>
/// 初期化
/// </summary>
void DebugCamera::Initialize(YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// ローカル拡縮
	scale_ = { 1.0f , 1.0f , 1.0f };

	// X,Y,Z軸回りのローカル回転角
	rotation_ = { 0.0f , 0.0f , 0.0f };

	// ローカル座標
	translation_ = { 0.0f , 0.0f , -50.0f };
}

/// <summary>
/// 更新処理
/// </summary>
void DebugCamera::Update()
{
	// マウスホイールを上回転させると、向いている方向にズームイン
	if (engine_->GetMouseWheelUp())
	{
		// 回転行列で進む方向を取得する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation_);
		Vector3 velocity = TransformNormal(Vector3{ 0.0f,0.0f,1.0f }, rotateMatrix);

		// 移動する
		translation_ += velocity;
	}
	
	// マウスホイールを下回転させると、向いている方向にズームアウト
	if (engine_->GetMouseWheelDown())
	{
		// 回転行列で進む方向を取得する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation_);
		Vector3 velocity = TransformNormal(Vector3{ 0.0f,0.0f,-1.0f }, rotateMatrix);

		// 移動する
		translation_ += velocity;
	}

	// LShift + マウスホイールを押して動かすと、向いている方向の左右に移動できる
	if (engine_->GetKeyPress(DIK_LSHIFT) && engine_->GetMouseButtonPress(kMouseButtonCenter))
	{
		// マウスのベクトルと距離を取得し、正規化する
		Vector2 mouseVector = engine_->GetMouseVelocity();
		float mouseVectorLength = Length(mouseVector);
		mouseVector = Normalize(mouseVector);

		// 移動量制御
		const float kSpeedController = mouseVectorLength * 0.01f;

		// 回転行列で進む方向を取得する
		Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation_);
		Vector3 velocity = TransformNormal({-mouseVector.x , mouseVector.y , 0.0f}, rotateMatrix);

		// 移動する
		translation_ += velocity * kSpeedController;
	}
	else if (engine_->GetMouseButtonPress(kMouseButtonCenter))
	{
		// マウスホイールを押して動かすと、向いている方向を回転できる

		// マウスのベクトルと距離を取得し、正規化する
		Vector2 mouseVector = engine_->GetMouseVelocity();
		float mouseVectorLength = Length(mouseVector);
		mouseVector = Normalize(mouseVector);

		// 回転量制御
		const float kSpeedController = mouseVectorLength * 0.001f;

		// 回転量
		Vector2 rotationVelocity = mouseVector * kSpeedController;

		// 回転させる
		rotation_.x += rotationVelocity.y;
		rotation_.y += rotationVelocity.x;
	}


	
}