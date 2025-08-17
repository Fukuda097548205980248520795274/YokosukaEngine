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
	const float kShotParameterMax = 4.0f;

	// 発射パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;


	// 持ち上げ
	float lifftingFrames_[2] = { 0.0f , 0.5f };
	float lifftingValuesX_[2] = { 0.0f ,-std::numbers::pi_v<float> / 2.0f };
	float lifftingValuesY_[2] = { 0.0f , -std::numbers::pi_v<float> / 5.0f };
	float lifftingValuesZ_[2] = { 0.0f , std::numbers::pi_v<float> / 2.0f };
	float lifftungWeaponValuesY_[2] = { 0.0f , std::numbers::pi_v<float> / 3.0f };


	// 1振り
	float oneSwingFrames_[2] = { 1.0f , 1.3f };
	float oneSwingValues_[2] = { 0.0f , -std::numbers::pi_v<float> / 4.0f };
	float oneSwingArmValueX_[2] = { -std::numbers::pi_v<float> / 2.0f ,  -std::numbers::pi_v<float> / 2.0f };
	float oneSwingArmValueY_[2] = { -std::numbers::pi_v<float> / 5.0f , -std::numbers::pi_v<float> / 1.5f };
	float oneSwingArmValueZ_[2] = { std::numbers::pi_v<float> / 2.0f , std::numbers::pi_v<float> / 3.0f };


	// 2振り
	float twoSwingFrames_[2] = { 1.6f , 1.9f };
	float twoSwingValues_[2] = { -std::numbers::pi_v<float> / 4.0f , std::numbers::pi_v<float> / 4.0f };
	float twoSwingArmValueX_[2] = { -std::numbers::pi_v<float> / 2.0f ,  -std::numbers::pi_v<float> / 2.0f };
	float twoSwingArmValueY_[2] = { -std::numbers::pi_v<float> / 1.5f , std::numbers::pi_v<float> / 5.0f };
	float twoSwingArmValueZ_[2] = { std::numbers::pi_v<float> / 3.0f ,  std::numbers::pi_v<float> / 3.0f };


	// 3振り
	float threeSwingFrames_[2] = { 2.2f , 2.7f };
	float threeSwingValues_[2] = { -std::numbers::pi_v<float> / 4.0f , -std::numbers::pi_v<float> *2.0f };
	float threeSwingArmValueX_[2] = { -std::numbers::pi_v<float> / 2.0f ,  -std::numbers::pi_v<float> / 2.0f };
	float threeSwingArmValueY_[2] = { std::numbers::pi_v<float> / 5.0f , -std::numbers::pi_v<float> / 1.5f };
	float threeSwingArmValueZ_[2] = { std::numbers::pi_v<float> / 3.0f , std::numbers::pi_v<float> / 3.0f };
	float threeSwingWeaponValuesY_[2] = { 0.0f , std::numbers::pi_v<float> / 2.0f };


	// 元に戻す
	float returnFrames_[2] = { 3.4f , 3.9f };
	float returnSwingArmValueX_[2] = { -std::numbers::pi_v<float> / 2.0f ,  0.0f };
	float returnSwingArmValueY_[2] = { -std::numbers::pi_v<float> / 1.5f , 0.0f };
	float returnSwingArmValueZ_[2] = { std::numbers::pi_v<float> / 3.0f , 0.0f };
	float returnSwingWeaponValuesY_[2] = { std::numbers::pi_v<float> / 2.0f , 0.0f };
};

