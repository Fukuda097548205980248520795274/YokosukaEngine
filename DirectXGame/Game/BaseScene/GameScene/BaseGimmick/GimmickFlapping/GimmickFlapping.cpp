#include "GimmickFlapping.h"

/// <summary>
/// 初期化
/// </summary>
void GimmickFlapping::Initialize(WorldTransform* worldTransform, float parameterVelocity)
{
	// 基底クラスの初期化
	BaseGimmick::Initialize(worldTransform, parameterVelocity);


	// パラメータ最大値を設定する
	parameterMax_ = std::numbers::pi_v<float> *2.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void GimmickFlapping::Update()
{
	// 基底クラスの更新処理
	BaseGimmick::Update();

	// ふわふわさせる
	if (axis_ == kX)
	{
		worldTransform_->rotation_.x = startRotation_ + (std::sin(parameter_) * amplitude_);
	}
	else if(axis_ == kY)
	{
		worldTransform_->rotation_.y = startRotation_ + (std::sin(parameter_) * amplitude_);
	}
	else if (axis_ = kZ)
	{
		worldTransform_->rotation_.z = startRotation_ + (std::sin(parameter_) * amplitude_);
	}
}