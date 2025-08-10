#pragma once
#include "../BaseEnemyFairyBehavior.h"

class EnemyFairyBehaviorShot : public BaseEnemyFairyBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyFairyBehaviorShot(EnemyFairy* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;


private:


	// 発射パラメータ
	float shotParameter_ = 0.0f;

	// 発射パラメータの最大値
	const float kShotParameterMax = 2.0f;

	// 発射パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;


	// 小さく下がる
	float littleDownFrames_[2] = { 0.0f , 0.5f };
	float littleDownValues_[2] = { 0.0f , -1.0f };

	// 大きく上がる
	float veryUpFrames_[2] = { 0.5f , 1.5f };
	float veryUpValues_[2] = { -1.0f , 3.0f };

	// 回転する
	float rotateFrames_[2] = { 0.5f , 2.0f };
	float rotateValues_[2] = { 0.0f , std::numbers::pi_v<float> * 2.0f };

	// 下がる
	float downFrames_[2] = { 1.5f , 2.0f };
	float downValues_[2] = { 3.0f , 0.0f };
};

