#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class BaseGimmick
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(WorldTransform* worldTransform , float parameterVelocity);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// パラメータ速度のSetter
	/// </summary>
	/// <param name="parameterMax"></param>
	void SetParameterVelocity(float parameterVelocity) { parameterVelocity_ = parameterVelocity; }


protected:

	// ワールドトランスフォーム
	WorldTransform* worldTransform_ = nullptr;

	// パラメータ
	float parameter_ = 0.0f;

	// パラメータの速度
	float parameterVelocity_ = 0.0f;

	// パラメータ最大値
	float parameterMax_ = 0.0f;
};

