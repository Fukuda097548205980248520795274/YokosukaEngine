#pragma once
#include "../BaseEnemyButterflyBehavior.h"

class EnemyButterflyBehaviorShot : public BaseEnemyButterflyBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyButterflyBehaviorShot(EnemyButterfly* enemy);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;


private:

	/// <summary>
	/// 発射アクション
	/// </summary>
	void ShotAction();

	// 発射動作パラメータ
	float parameter_ = 0.0f;

	// 発射動作の初期化時の角度
	float currentRotation_ = 0.0f;


	// 発射操作初期回転のパラメータ
	const float kShotActionStartRotationParameter[2] = { 0.0f , 0.5f };

	// 発射動作初期回転
	const float kShotActionStartRotation[3] = { 0.0f ,-2.0f,2.0f };


	// 発射操作回転のパラメータ
	const float kShotActionRotationParameter[2] = { 0.75f , 1.00f };

	// 発射動作回転
	const float kShotActionRotation[3] = { 0.0f,1.0f,-1.0f };
	

	// 発射操作終了回転のパラメータ
	const float kShotActionEndRotationParameter[2] = { 2.50f , 3.00f };

	// 発射動作終了回転
	const float kShotActionEndRotation[3] = { 0.0f,0.0f,0.0f };


	// 発射したかどうか
	bool isShot_ = false;



	// 発射パラメータ
	float shotParameter_ = 0.0f;

	// 発射パラメータの最大値
	const float kShotParameterMax = 3.0f;

	// 発射パラメータの速度
	const float kShotParameterVelocity = 1.0f / 60.0f;
};

