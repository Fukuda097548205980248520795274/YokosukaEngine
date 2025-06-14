#include "MainCamera.h"
#include "../GameScene/Player/Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="screenWidth">画面の横幅</param>
/// <param name="screenHeight">画面の縦幅</param>
void MainCamera::Initialize(float screenWidth, float screenHeight)
{
	// カメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(screenWidth, screenHeight);
}

/// <summary>
/// 更新処理
/// </summary>
void MainCamera::Update()
{
	// 追従対象がいるときだけ
	if (target_)
	{
		// 追従対象のワールドトランスフォーム
		const WorldTransform* targetWorldTransform = target_->GetWorldTransform();

		// 追従対象とオフセットからカメラの位置で、目標座標を計算
		goalPosition_ = targetWorldTransform->translation_ + targetOffset_ + target_->GetVelocity() * kVelocityBias;

		// 座標補間によりゆったり追従
		camera3d_->translation_ = Lerp(camera3d_->translation_, goalPosition_, kInterpolationRate);

		// 追従対象が画面外に出ないようにする
		camera3d_->translation_.x = std::max(camera3d_->translation_.x, targetWorldTransform->translation_.x + targetMargin_.left);
		camera3d_->translation_.x = std::min(camera3d_->translation_.x, targetWorldTransform->translation_.x + targetMargin_.right);
		camera3d_->translation_.y = std::max(camera3d_->translation_.y, targetWorldTransform->translation_.y + targetMargin_.bottom);
		camera3d_->translation_.y = std::min(camera3d_->translation_.y, targetWorldTransform->translation_.y + targetMargin_.top);
	}


	// 移動範囲を越えないようにする
	camera3d_->translation_.x = std::max(camera3d_->translation_.x, movableArea_.left);
	camera3d_->translation_.x = std::min(camera3d_->translation_.x, movableArea_.right);
	camera3d_->translation_.y = std::max(camera3d_->translation_.y, movableArea_.bottom);
	camera3d_->translation_.y = std::min(camera3d_->translation_.y, movableArea_.top);

	// 3Dカメラ更新
	camera3d_->UpdateMatrix();
}

/// <summary>
/// リセット関数
/// </summary>
void MainCamera::Reset()
{
	// 追従対象がいるときだけ
	if (target_)
	{
		// 追従対象のワールドトランスフォーム
		const WorldTransform* targetWorldTransform = target_->GetWorldTransform();

		// 追従対象とオフセットからカメラの位置を計算
		camera3d_->translation_ = targetWorldTransform->translation_ + targetOffset_;
	}
}