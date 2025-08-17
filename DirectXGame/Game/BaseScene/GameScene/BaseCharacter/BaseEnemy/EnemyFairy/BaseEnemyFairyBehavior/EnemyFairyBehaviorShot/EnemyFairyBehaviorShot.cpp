#include "EnemyFairyBehaviorShot.h"
#include "../../EnemyFairy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyFairyBehaviorShot::EnemyFairyBehaviorShot(EnemyFairy* enemy) : BaseEnemyFairyBehavior(enemy)
{
	// 位置を求める
	littleDownValues_[0] += enemy_->GetBodyWorldTransform()->translation_.y;
	littleDownValues_[1] += enemy_->GetBodyWorldTransform()->translation_.y;

	littleDownArmRValuesY_[0] += enemy_->GetArmRWorldTransform()->rotation_.y;
	littleDownArmRValuesY_[1] += enemy_->GetArmRWorldTransform()->rotation_.y;

	littleDownArmRValuesZ_[0] += enemy_->GetArmRWorldTransform()->rotation_.z;
	littleDownArmRValuesZ_[1] += enemy_->GetArmRWorldTransform()->rotation_.z;

	swingUpValueY_[0] += enemy_->GetArmRWorldTransform()->rotation_.y;
	swingUpValueY_[1] += enemy_->GetArmRWorldTransform()->rotation_.y;

	swingUpWeaponValueZ_[0] += enemy_->GetWeaponWorldTransform()->rotation_.z;
	swingUpWeaponValueZ_[1] += enemy_->GetWeaponWorldTransform()->rotation_.z;

	veryUpValues_[0] += enemy_->GetBodyWorldTransform()->translation_.y;
	veryUpValues_[1] += enemy_->GetBodyWorldTransform()->translation_.y;

	rotateValues_[0] += enemy_->GetBodyWorldTransform()->rotation_.y;
	rotateValues_[1] += enemy_->GetBodyWorldTransform()->rotation_.y;

	downValues_[0] += enemy_->GetBodyWorldTransform()->translation_.y;
	downValues_[1] += enemy_->GetBodyWorldTransform()->translation_.y;

	downArmRValuesZ_[0] += enemy_->GetArmRWorldTransform()->rotation_.z;
	downArmRValuesZ_[1] += enemy_->GetArmRWorldTransform()->rotation_.z;

	downWeaponValueZ_[0] += enemy_->GetWeaponWorldTransform()->rotation_.z;
	downWeaponValueZ_[1] += enemy_->GetWeaponWorldTransform()->rotation_.z;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyFairyBehaviorShot::Update()
{
	// ゲームタイマーを取得する
	const float* gameTimer = enemy_->GetGameTimer();

	// 本体のワールドトランスフォーム
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();

	// 右腕のワールドトランスフォーム
	WorldTransform* armRWorldTransform = enemy_->GetArmRWorldTransform();

	// 武器のワールドトランスフォーム
	WorldTransform* weaponWorldTransform = enemy_->GetWeaponWorldTransform();


	// 発射パラメータを進める
	shotParameter_ += kShotParameterVelocity * (*gameTimer);
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);


	if (shotParameter_ >= littleDownFrames_[0] && shotParameter_ <= littleDownFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - littleDownFrames_[0]) / (littleDownFrames_[1] - littleDownFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);

		bodyWorldTransform->translation_.y = Lerp(littleDownValues_[0], littleDownValues_[1], easing);

		armRWorldTransform->rotation_.y = Lerp(littleDownArmRValuesY_[0], littleDownArmRValuesY_[1], easing);
		armRWorldTransform->rotation_.z = Lerp(littleDownArmRValuesZ_[0], littleDownArmRValuesZ_[1], easing);
	}

	if (shotParameter_ >= swingUpFrames_[0] && shotParameter_ <= swingUpFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - swingUpFrames_[0]) / (swingUpFrames_[1] - swingUpFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);

		armRWorldTransform->rotation_.y = Lerp(swingUpValueY_[0], swingUpValueY_[1], easing);
		weaponWorldTransform->rotation_.z = Lerp(swingUpWeaponValueZ_[0], swingUpWeaponValueZ_[1], easing);
	}

	if (shotParameter_ >= veryUpFrames_[0] && shotParameter_ <= veryUpFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - veryUpFrames_[0]) / (veryUpFrames_[1] - veryUpFrames_[0]);
		float easing = t < 0.5 ? 2.0f * t * t : 1 - std::powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;
		bodyWorldTransform->translation_.y = Lerp(veryUpValues_[0], veryUpValues_[1], easing);
	}

	if (shotParameter_ >= rotateFrames_[0] && shotParameter_ <= rotateFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - rotateFrames_[0]) / (rotateFrames_[1] - rotateFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);
		bodyWorldTransform->rotation_.y = Lerp(rotateValues_[0], rotateValues_[1], easing);
	}

	if (shotParameter_ >= downFrames_[0] && shotParameter_ <= downFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - downFrames_[0]) / (downFrames_[1] - downFrames_[0]);
		float easing = std::powf(t, 2.0f);
		bodyWorldTransform->translation_.y = Lerp(downValues_[0], downValues_[1], easing);
		armRWorldTransform->rotation_.z = Lerp(downArmRValuesZ_[0], downArmRValuesZ_[1], easing);
		weaponWorldTransform->rotation_.z = Lerp(downWeaponValueZ_[0], downWeaponValueZ_[1], easing);
	}


	// 発射パラメータが最大値で終了する
	if (shotParameter_ >= kShotParameterMax)
	{
		isFinished_ = true;
		
		bodyWorldTransform->rotation_.y = rotateValues_[0];

		armRWorldTransform->rotation_.y = littleDownArmRValuesY_[0];
		armRWorldTransform->rotation_.z = littleDownArmRValuesZ_[0];

		weaponWorldTransform->rotation_.z = swingUpWeaponValueZ_[0];
	}
}