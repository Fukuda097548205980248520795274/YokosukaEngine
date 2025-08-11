#include "EnemyDevilBehaviorShot.h"
#include "../../EnemyDevil.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy"></param>
EnemyDevilBehaviorShot::EnemyDevilBehaviorShot(EnemyDevil* enemy) : BaseEnemyDevilBehavior(enemy)
{
	lifftingValuesX_[0] += enemy_->GetArmRWorldTransform()->rotation_.x;
	lifftingValuesX_[1] += enemy_->GetArmRWorldTransform()->rotation_.x;

	lifftingValuesY_[0] += enemy_->GetArmRWorldTransform()->rotation_.y;
	lifftingValuesY_[1] += enemy_->GetArmRWorldTransform()->rotation_.y;

	lifftingValuesZ_[0] += enemy_->GetArmRWorldTransform()->rotation_.z;
	lifftingValuesZ_[1] += enemy_->GetArmRWorldTransform()->rotation_.z;

	lifftungWeaponValuesY_[0] += enemy_->GetWeaponWorldTransform()->rotation_.y;
	lifftungWeaponValuesY_[1] += enemy_->GetWeaponWorldTransform()->rotation_.y;



	oneSwingValues_[0] += enemy_->GetBodyWorldTransform()->rotation_.y;
	oneSwingValues_[1] += enemy_->GetBodyWorldTransform()->rotation_.y;

	oneSwingArmValueX_[0] += enemy_->GetArmRWorldTransform()->rotation_.x;
	oneSwingArmValueX_[1] += enemy_->GetArmRWorldTransform()->rotation_.x;

	oneSwingArmValueY_[0] += enemy_->GetArmRWorldTransform()->rotation_.y;
	oneSwingArmValueY_[1] += enemy_->GetArmRWorldTransform()->rotation_.y;

	oneSwingArmValueZ_[0] += enemy_->GetArmRWorldTransform()->rotation_.z;
	oneSwingArmValueZ_[1] += enemy_->GetArmRWorldTransform()->rotation_.z;



	twoSwingValues_[0] += enemy_->GetBodyWorldTransform()->rotation_.y;
	twoSwingValues_[1] += enemy_->GetBodyWorldTransform()->rotation_.y;

	twoSwingArmValueX_[0] += enemy_->GetArmRWorldTransform()->rotation_.x;
	twoSwingArmValueX_[1] += enemy_->GetArmRWorldTransform()->rotation_.x;

	twoSwingArmValueY_[0] += enemy_->GetArmRWorldTransform()->rotation_.y;
	twoSwingArmValueY_[1] += enemy_->GetArmRWorldTransform()->rotation_.y;

	twoSwingArmValueZ_[0] += enemy_->GetArmRWorldTransform()->rotation_.z;
	twoSwingArmValueZ_[1] += enemy_->GetArmRWorldTransform()->rotation_.z;



	threeSwingValues_[0] += enemy_->GetBodyWorldTransform()->rotation_.y;
	threeSwingValues_[1] += enemy_->GetBodyWorldTransform()->rotation_.y;

	threeSwingArmValueX_[0] += enemy_->GetArmRWorldTransform()->rotation_.x;
	threeSwingArmValueX_[1] += enemy_->GetArmRWorldTransform()->rotation_.x;

	threeSwingArmValueY_[0] += enemy_->GetArmRWorldTransform()->rotation_.y;
	threeSwingArmValueY_[1] += enemy_->GetArmRWorldTransform()->rotation_.y;

	threeSwingArmValueZ_[0] += enemy_->GetArmRWorldTransform()->rotation_.z;
	threeSwingArmValueZ_[1] += enemy_->GetArmRWorldTransform()->rotation_.z;

	threeSwingWeaponValuesY_[0] += enemy_->GetWeaponWorldTransform()->rotation_.y;
	threeSwingWeaponValuesY_[1] += enemy_->GetWeaponWorldTransform()->rotation_.y;



	returnSwingArmValueX_[0] += enemy_->GetArmRWorldTransform()->rotation_.x;
	returnSwingArmValueX_[1] += enemy_->GetArmRWorldTransform()->rotation_.x;

	returnSwingArmValueY_[0] += enemy_->GetArmRWorldTransform()->rotation_.y;
	returnSwingArmValueY_[1] += enemy_->GetArmRWorldTransform()->rotation_.y;

	returnSwingArmValueZ_[0] += enemy_->GetArmRWorldTransform()->rotation_.z;
	returnSwingArmValueZ_[1] += enemy_->GetArmRWorldTransform()->rotation_.z;

	returnSwingWeaponValuesY_[0] += enemy_->GetWeaponWorldTransform()->rotation_.y;
	returnSwingWeaponValuesY_[1] += enemy_->GetWeaponWorldTransform()->rotation_.y;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyDevilBehaviorShot::Update()
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


	if (shotParameter_ >= lifftingFrames_[0] && shotParameter_ <= lifftingFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - lifftingFrames_[0]) / (lifftingFrames_[1] - lifftingFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);

		armRWorldTransform->rotation_.z = Lerp(lifftingValuesZ_[0], lifftingValuesZ_[1], easing);
		armRWorldTransform->rotation_.y = Lerp(lifftingValuesY_[0], lifftingValuesY_[1], easing);
		armRWorldTransform->rotation_.x = Lerp(lifftingValuesX_[0], lifftingValuesX_[1], easing);

		weaponWorldTransform->rotation_.y = Lerp(lifftungWeaponValuesY_[0], lifftungWeaponValuesY_[1], easing);
	}

	if (shotParameter_ >= oneSwingFrames_[0] && shotParameter_ <= oneSwingFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - oneSwingFrames_[0]) / (oneSwingFrames_[1] - oneSwingFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);

		armRWorldTransform->rotation_.x = Lerp(oneSwingArmValueX_[0], oneSwingArmValueX_[1], easing);
		armRWorldTransform->rotation_.y = Lerp(oneSwingArmValueY_[0], oneSwingArmValueY_[1], easing);
		armRWorldTransform->rotation_.z = Lerp(oneSwingArmValueZ_[0], oneSwingArmValueZ_[1], easing);

		bodyWorldTransform->rotation_.y = Lerp(oneSwingValues_[0], oneSwingValues_[1], easing);
	}

	if (shotParameter_ >= twoSwingFrames_[0] && shotParameter_ <= twoSwingFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - twoSwingFrames_[0]) / (twoSwingFrames_[1] - twoSwingFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);

		armRWorldTransform->rotation_.x = Lerp(twoSwingArmValueX_[0], twoSwingArmValueX_[1], easing);
		armRWorldTransform->rotation_.y = Lerp(twoSwingArmValueY_[0], twoSwingArmValueY_[1], easing);
		armRWorldTransform->rotation_.z = Lerp(twoSwingArmValueZ_[0], twoSwingArmValueZ_[1], easing);

		bodyWorldTransform->rotation_.y = Lerp(twoSwingValues_[0], twoSwingValues_[1], easing);
	}

	if (shotParameter_ >= threeSwingFrames_[0] && shotParameter_ <= threeSwingFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - threeSwingFrames_[0]) / (threeSwingFrames_[1] - threeSwingFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);

		armRWorldTransform->rotation_.x = Lerp(threeSwingArmValueX_[0], threeSwingArmValueX_[1], easing);
		armRWorldTransform->rotation_.y = Lerp(threeSwingArmValueY_[0], threeSwingArmValueY_[1], easing);
		armRWorldTransform->rotation_.z = Lerp(threeSwingArmValueZ_[0], threeSwingArmValueZ_[1], easing);

		bodyWorldTransform->rotation_.y = Lerp(threeSwingValues_[0], threeSwingValues_[1], easing);

		weaponWorldTransform->rotation_.y = Lerp(threeSwingWeaponValuesY_[0], threeSwingWeaponValuesY_[1], easing);
	}

	if (shotParameter_ >= returnFrames_[0] && shotParameter_ <= returnFrames_[1])
	{
		// 媒介変数
		float t = (shotParameter_ - returnFrames_[0]) / (returnFrames_[1] - returnFrames_[0]);
		float easing = 1.0f - std::powf(1.0f - t, 2.0f);

		armRWorldTransform->rotation_.x = Lerp(returnSwingArmValueX_[0], returnSwingArmValueX_[1], easing);
		armRWorldTransform->rotation_.y = Lerp(returnSwingArmValueY_[0], returnSwingArmValueY_[1], easing);
		armRWorldTransform->rotation_.z = Lerp(returnSwingArmValueZ_[0], returnSwingArmValueZ_[1], easing);

		weaponWorldTransform->rotation_.y = Lerp(returnSwingWeaponValuesY_[0], returnSwingWeaponValuesY_[1], easing);
	}


	// 発射パラメータが最大値で終了する
	if (shotParameter_ >= kShotParameterMax)
	{
		isFinished_ = true;

		bodyWorldTransform->rotation_.y = oneSwingValues_[0];

		armRWorldTransform->rotation_.y = lifftingValuesY_[0];
		armRWorldTransform->rotation_.z = lifftingValuesZ_[0];
		armRWorldTransform->rotation_.x = lifftingValuesX_[0];

		weaponWorldTransform->rotation_.y = lifftungWeaponValuesY_[0];
	}
}