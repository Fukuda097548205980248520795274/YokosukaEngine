#include "GimmickFloating.h"

/// <summary>
/// 初期化
/// </summary>
void GimmickFloating::Initialize(WorldTransform* worldTransform, float parameterVelocity)
{
	// 基底クラスの初期化
	BaseGimmick::Initialize(worldTransform, parameterVelocity);


	// パラメータ最大値を設定する
	parameterMax_ = std::numbers::pi_v<float> *2.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void GimmickFloating::Update()
{
	// 基底クラスの更新処理
	BaseGimmick::Update();

	// ふわふわさせる
	worldTransform_->translation_.y = std::sin(parameter_) * amplitude_;
}