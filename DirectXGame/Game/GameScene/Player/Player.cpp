#include "Player.h"

/// <summary>
/// Initialize
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// Check for nullptr
	assert(engine);
	assert(camera3d);

	// Receive arguments
	engine_ = engine;
	camera3d_ = camera3d;


	// Create and initialize worldTransform
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// Create and initialize uvTransform
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// Load the modelData
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Player", "Player.obj");
}

/// <summary>
/// Update
/// </summary>
void Player::Update()
{
	// 入力操作
	Input();

	// Update worldTransform
	worldTransform_->UpdateWorldMatrix();

	// Update uvTransform
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// Draw
/// </summary>
void Player::Draw()
{
	// Draw the model
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
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
	// コントローラが使えるかどうか
	if (engine_->IsGamepadEnable(0))
	{
		// デッドゾーン
		const float kDeadZone = 0.7f;

		// 移動フラグ
		bool isMove = false;

		// 移動量
		Vector3 move =
		{
			engine_->GetGamepadLeftStick(0).x ,
			0.0f,
			engine_->GetGamepadLeftStick(0).y
		};

		// デッドゾーンを越えないときは処理しない
		if (Length(move) >= kDeadZone)
		{
			isMove = true;
		}


		// 移動しているときの処理
		if (isMove)
		{
			// 早さ
			const float speed = 0.3f;

			// 移動量に早さを反映する
			move = Normalize(move) * speed;

			// メインカメラの向きで移動方向を変える
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(mainCamera_->rotation_.y);
			move = TransformNormal(move, rotateMatrix);

			// 移動させる
			worldTransform_->translation_ += move;

			// 移動方向に合わせて回転する目標角度
			toRotationY_ = std::atan2(move.x, move.z);
		}
	}

	// 最短角度補間
	worldTransform_->rotation_.y = LerpShortAngle(worldTransform_->rotation_.y, toRotationY_, 0.2f);
}