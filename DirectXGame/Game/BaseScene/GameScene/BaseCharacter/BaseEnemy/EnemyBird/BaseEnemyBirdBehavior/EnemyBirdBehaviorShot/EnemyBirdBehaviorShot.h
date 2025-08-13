#pragma once
#include "../BaseEnemyBirdBehavior.h"

class EnemyBirdBehaviorShot : public BaseEnemyBirdBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyBirdBehaviorShot(EnemyBird* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;


private:


	// 発射パラメータ
	float shotParameter_ = 0.0f;

	// 発射パラメータの最大値
	const float kShotParameterMax = 2.5f;

	// 発射パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;



	/*-------------
	    傾き
	-------------*/

	float wingInclinationFrames_[2] = { 0.0f , 0.5f };
	float topWingRInclinationValues_[2] = { 0.0f , -0.5f };
	float topWingLInclinationValues_[2] = { 0.0f , -0.5f };


	/*--------------
	    一回転傾き
	--------------*/

	float oneSwingInclinationFrames_[2] = { 0.9f , 1.25f };
	float topWingROneSwingInclinationValues_[2] = { -0.5f , 0.75f };
	float topWingLOneSwingInclinationValues_[2] = { -0.5f , 0.75f };


	/*----------
	    1回転
	----------*/

	// 回転するフレーム
	float rotateFrames[2] = { 1.0f , 2.0f };
	float rotateValues[2] = { 0.0f , std::numbers::pi_v<float> *2.0f };


	/*------------
	    元に戻す
	------------*/

	float returnFrames_[2] = { 1.75f , 2.5f };
	float topWingRReturnValues_[2] = { 0.75f , 0.0f };
	float topWingLReturnValues_[2] = { 0.75f , 0.0f };
};

