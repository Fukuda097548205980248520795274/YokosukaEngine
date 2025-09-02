#include "ScaleActionGamepad.h"
#include "../../Enemy/Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="worldTransform"></param>
void ScaleActionGamepad::Initialize(const YokosukaEngine* engine, WorldTransform* worldTransform)
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
void ScaleActionGamepad::Update()
{
	// 操作フラグを初期化
	isOperation_ = false;

	// ターゲットリストの中身がないとき
	if (targets_.size() == 0)
		return;

	if (engine_->IsGamepadEnable(0) == false)
		return;

	// 右のショルダーボタンを押しているかどうか
	if (engine_->GetGamepadButtonPress(0, XINPUT_GAMEPAD_RIGHT_SHOULDER) == false)
		return;


	// 進む向き
	Vector2 stickDirection = engine_->GetGamepadLeftStick(0);
	if (stickDirection.x <= deadzone_ && stickDirection.x >= -deadzone_ &&
		stickDirection.y <= deadzone_ && stickDirection.y >= -deadzone_)
		return;

	// プレイヤーの向く後ろ向きに進む
	Vector3 direction = Normalize(TransformNormal(Vector3(0.0f, 0.0f, -1.0f), worldTransform_->worldMatrix_));

	if (Dot(Vector2(direction.x, direction.y), stickDirection) <= 0.0f)
	{
		worldTransform_->translation_.x += direction.x * -speed_;
		worldTransform_->translation_.y += direction.y * -speed_;
	}
	else
	{
		worldTransform_->translation_.x += direction.x * speed_;
		worldTransform_->translation_.y += direction.y * speed_;
	}

	direction_ = -1.0f * direction;

	for (Enemy* target : targets_)
	{
		if (Dot(Vector2(direction.x, direction.y), stickDirection) <= 0.0f)
		{
			Vector3 size = target->GetScale() + Vector3(-0.01f, -0.01f, -0.01f);

			target->SetScale(Vector3(std::max(size.x, kMinSize), std::max(size.y, kMinSize), std::max(size.z, kMinSize)));
		}
		else
		{
			Vector3 size = target->GetScale() + Vector3(0.01f, 0.01f, 0.01f);

			target->SetScale(Vector3(std::min(size.x, kMaxSize), std::min(size.y, kMaxSize), std::min(size.z, kMaxSize)));
		}
	}

	

	// 操作フラグがたつ
	isOperation_ = true;
}