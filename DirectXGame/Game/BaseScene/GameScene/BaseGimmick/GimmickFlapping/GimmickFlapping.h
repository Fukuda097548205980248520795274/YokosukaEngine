#pragma once
#include "../BaseGimmick.h"

class GimmickFlapping : public BaseGimmick
{
public:

	// 回転させる軸
	enum RotationAxis
	{
		kX,
		kY,
		kZ
	};


public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform* worldTransform, float parameterVelocity) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 振幅のSetter
	/// </summary>
	/// <param name="amplitude"></param>
	void SetAmplitude(float amplitude) { amplitude_ = amplitude; }

	/// <summary>
	/// 初期回転値のSetter
	/// </summary>
	/// <param name="startRotation"></param>
	void SetStartRotation(float startRotation) { startRotation_ = startRotation; }

	/// <summary>
	/// 回転軸のGetter
	/// </summary>
	/// <param name="axis"></param>
	void SetRotationAxis(RotationAxis axis) { axis_ = axis; }


private:

	// 振幅
	float amplitude_ = 0.3f;

	// 回転させる軸
	RotationAxis axis_ = kX;

	// 初期回転値
	float startRotation_ = 0.0f;
};

