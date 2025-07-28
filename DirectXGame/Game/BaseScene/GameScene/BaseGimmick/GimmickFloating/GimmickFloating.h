#pragma once
#include "../BaseGimmick.h"

class GimmickFloating : public BaseGimmick
{
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


private:

	// 振幅
	float amplitude_ = 0.3f;
};

