#include "MoveActionGamepad.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="worldTransform"></param>
void MoveActionGamepad::Initialize(const YokosukaEngine* engine,WorldTransform* worldTransform)
{
	// nullptrチェック
	assert(engine);
	assert(worldTransform);

	// 引数を受け取る
	engine_ = engine;
	worldTransform_ = worldTransform;
}

/// <summary>
/// 更新処理
/// </summary>
void MoveActionGamepad::Update()
{
	if (engine_->IsGamepadEnable(0) == false)
		return;


	// 進む向き
	Vector2 direction = engine_->GetGamepadLeftStick(0);
	if (direction.x <= deadzone_ && direction.x >= -deadzone_ && direction.y <= deadzone_ && direction.y >= -deadzone_)
		return;

	// 移動速度
	worldTransform_->translation_.x += direction.x * speed_;
	worldTransform_->translation_.y += direction.y * speed_;

	// 進む方向を向く
	worldTransform_->rotation_.y = std::atan2(direction.x, 0.0f);
	float length = std::sqrt(std::pow(direction.x, 2.0f));
	worldTransform_->rotation_.x = std::atan2(-direction.y, length);
}