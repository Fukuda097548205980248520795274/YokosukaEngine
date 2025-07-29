#include "BaseGimmick.h"

/// <summary>
/// 初期化
/// </summary>
void BaseGimmick::Initialize(WorldTransform* worldTransform, float parameterVelocity)
{
	// nullptrチェック
	assert(worldTransform);

	// 引数を受け取る
	worldTransform_ = worldTransform;
	parameterVelocity_ = parameterVelocity;
}

/// <summary>
/// 更新処理
/// </summary>
void BaseGimmick::Update()
{
	// パラメータを加算する
	if (gameTimer_)
	{
		parameter_ += parameterVelocity_ * (*gameTimer_);
	}
	else
	{
		parameter_ += parameterVelocity_;
	}

	// 範囲を制限する
	parameter_ = std::fmod(parameter_, parameterMax_);
}