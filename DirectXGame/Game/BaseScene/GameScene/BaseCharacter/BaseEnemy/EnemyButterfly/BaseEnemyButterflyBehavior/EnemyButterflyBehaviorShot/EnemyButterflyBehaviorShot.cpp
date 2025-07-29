#include "EnemyButterflyBehaviorShot.h"
#include "../../EnemyButterfly.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyButterflyBehaviorShot::EnemyButterflyBehaviorShot(EnemyButterfly* enemy) : BaseEnemyButterflyBehavior(enemy)
{

}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterflyBehaviorShot::Update()
{
	// 発射パラメータを進める
	shotParameter_ += kShotParameterVelocity;
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);

	// 発射パラメータが終了したら、通所ビヘイビアに遷移する
	if (shotParameter_ >= kShotParameterMax)
	{
		return;
	}

	// 発射アクション
	ShotAction();
}

/// <summary>
/// 発射アクション
/// </summary>
void EnemyButterflyBehaviorShot::ShotAction()
{
	// 発射ビヘイビアが終了するまでパラメータを進める
	if (parameter_ >= kShotParameterMax)
		return;


	// ワールドトランスフォームを取得する
	WorldTransform* worldTransform = enemy_->GetWorldTransform();
	WorldTransform* bodyWorldTransform = enemy_->GetBodyWorldTransform();
	WorldTransform* wingRWorldTransform = enemy_->GetWingRWorldTransform();
	WorldTransform* wingLWorldTransform = enemy_->GetWingLWorldTransform();

	// ターゲットのインスタンスを取得する
	const BaseCharacter* target = enemy_->GetTarget();


	// パラメータを進める
	parameter_ += kShotParameterVelocity;
	parameter_ = std::min(parameter_, kShotParameterMax);

	// 羽を広げる
	if (parameter_ >= kShotActionStartRotationParameter[0] && parameter_ <= kShotActionStartRotationParameter[1])
	{
		// 補間
		float t = (parameter_ - kShotActionStartRotationParameter[0]) / (kShotActionStartRotationParameter[1] - kShotActionStartRotationParameter[0]);
		float easing = 1.0f - std::powf(1.0f - t, 3);

		wingRWorldTransform->rotation_.y = Lerp(wingRWorldTransform->rotation_.y, kShotActionStartRotation[EnemyButterfly::kWingR], t);
		wingLWorldTransform->rotation_.y = Lerp(wingLWorldTransform->rotation_.y, kShotActionStartRotation[EnemyButterfly::kWingL], t);
	}

	// 羽を閉じる
	if (parameter_ >= kShotActionRotationParameter[0] && parameter_ <= kShotActionRotationParameter[1])
	{
		// 補間
		float t = (parameter_ - kShotActionRotationParameter[0]) / (kShotActionRotationParameter[1] - kShotActionRotationParameter[0]);
		float easing = 1.0f - std::powf(1.0f - t, 3);

		wingRWorldTransform->rotation_.y = Lerp(kShotActionStartRotation[EnemyButterfly::kWingR], kShotActionRotation[EnemyButterfly::kWingR], t);
		wingLWorldTransform->rotation_.y = Lerp(kShotActionStartRotation[EnemyButterfly::kWingL], kShotActionRotation[EnemyButterfly::kWingL], t);
	}

	// 羽を閉じ終えたら発射する
	if (parameter_ >= kShotActionRotationParameter[1])
	{
		if (isShot_ == false)
		{
			isShot_ = true;
			enemy_->BulletShot();
		}
	}

	// 羽を戻す
	if (parameter_ >= kShotActionEndRotationParameter[0] && parameter_ <= kShotActionEndRotationParameter[1])
	{
		// 補間
		float t = (parameter_ - kShotActionEndRotationParameter[0]) / (kShotActionEndRotationParameter[1] - kShotActionEndRotationParameter[0]);

		wingRWorldTransform->rotation_.y = Lerp(kShotActionRotation[EnemyButterfly::kWingR], kShotActionEndRotation[EnemyButterfly::kWingR], t);
		wingLWorldTransform->rotation_.y = Lerp(kShotActionRotation[EnemyButterfly::kWingL], kShotActionEndRotation[EnemyButterfly::kWingL], t);
	}


	// ターゲットの方向を向くようにする
	Vector3 toTarget = -1.0f * Normalize(target->GetWorldTransform()->translation_ - worldTransform->translation_);
	worldTransform->rotation_.y = std::atan2(toTarget.x, toTarget.z);
	float length = std::sqrt(std::pow(toTarget.x, 2.0f) + std::pow(toTarget.z, 2.0f));
	worldTransform->rotation_.x = std::atan2(-toTarget.y, length);
}