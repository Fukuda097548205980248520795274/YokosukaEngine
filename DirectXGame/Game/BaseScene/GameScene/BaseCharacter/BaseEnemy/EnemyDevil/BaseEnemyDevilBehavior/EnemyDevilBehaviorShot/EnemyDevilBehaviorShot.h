#pragma once
#include "../BaseEnemyDevilBehavior.h"

class EnemyDevilBehaviorShot : public BaseEnemyDevilBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDevilBehaviorShot(EnemyDevil* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;


private:


	// 発射パラメータ
	float shotParameter_ = 0.0f;

	// 発射パラメータの最大値
	const float kShotParameterMax = 3.0f;

	// 発射パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;


	// 1振り
	float oneSwingFrames_[2] = { 0.0f , 0.3f };
	float oneSwingValues_[2] = { 0.0f , -std::numbers::pi_v<float> / 2.0f };

	// 2振り
	float twoSwingFrames_[2] = { 0.6f , 0.9f };
	float twoSwingValues_[2] = { -std::numbers::pi_v<float> / 2.0f , std::numbers::pi_v<float> / 2.0f };

	// 3振り
	float threeSwingFrames_[2] = { 1.2f , 1.7f };
	float threeSwingValues_[2] = { -std::numbers::pi_v<float> / 2.0f , -std::numbers::pi_v<float> *2.0f };
};

