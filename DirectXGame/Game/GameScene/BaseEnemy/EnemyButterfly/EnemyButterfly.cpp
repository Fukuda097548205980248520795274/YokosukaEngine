#include "EnemyButterfly.h"
#include "../../Player/Player.h"
#include "../../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void EnemyButterfly::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, const Player* target, GameScene* gameScene)
{
	// 基底クラス初期化
	BaseEnemy::Initialize(engine, camera3d, position, target, gameScene);
	worldTransform_->scale_ *= 2.0f;

	hitSize_ = { 2.0f , 2.0f , 1.0f };
	hp_ = 50;

	// 胴体の初期化
	models_[kBody].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform_->Initialize();
	models_[kBody].worldTransform_->SetParent(worldTransform_.get());
	models_[kBody].worldTransform_->translation_ = kStartPosition[kBody];
	models_[kBody].worldTransform_->rotation_ = kStartRotation[kBody];
	models_[kBody].uvTransform_ = std::make_unique<UvTransform>();
	models_[kBody].uvTransform_->Initialize();
	models_[kBody].modelHandle_ = engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/body", "body.obj");
	models_[kBody].color = Vector4(0.5f, 0.0f, 0.0f, 1.0f);

	// 右の羽の初期化
	models_[kWingR].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kWingR].worldTransform_->Initialize();
	models_[kWingR].worldTransform_->SetParent(models_[kBody].worldTransform_.get());
	models_[kWingR].worldTransform_->translation_ = kStartPosition[kWingR];
	models_[kWingR].worldTransform_->rotation_ = kStartRotation[kWingR];
	models_[kWingR].uvTransform_ = std::make_unique<UvTransform>();
	models_[kWingR].uvTransform_->Initialize();
	models_[kWingR].modelHandle_ = engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/wingR", "wingR.obj");
	models_[kWingR].color = Vector4(0.5f, 0.5f, 1.0f, 1.0f);

	// 左の羽の初期化
	models_[kWingL].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kWingL].worldTransform_->Initialize();
	models_[kWingL].worldTransform_->SetParent(models_[kBody].worldTransform_.get());
	models_[kWingL].worldTransform_->translation_ = kStartPosition[kWingL];
	models_[kWingL].worldTransform_->rotation_ = kStartRotation[kWingL];
	models_[kWingL].uvTransform_ = std::make_unique<UvTransform>();
	models_[kWingL].uvTransform_->Initialize();
	models_[kWingL].modelHandle_ = engine_->LoadModelData("./Resources/Models/enemy/enemyButterfly/wingL", "wingL.obj");
	models_[kWingL].color = Vector4(0.5f, 0.5f, 1.0f, 1.0f);


	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();


	// 浮遊ギミック初期化
	GimmickFloatingInitialize();
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterfly::Update()
{
	// 次のビヘイビアの予定があるとき
	if (requestBehavior_ != kNothing)
	{
		// ビヘイビアを変更する
		behavior_ = requestBehavior_;

		// 初期化する
		switch (behavior_)
		{
		case kNormal:
		default:
			// 通常

			BehaviorNormalInitialize();

			break;

		case kShot:
			// 発射

			BehaviorShotInitialize();

			break;
		}

		// 予定を消す
		requestBehavior_ = kNothing;
	}

	// 現在のビヘイビアの更新処理
	switch (behavior_)
	{
	case kNormal:
	default:
		// 通常

		BehaviorNormalUpdate();

		break;

	case kShot:
		// 発射

		BehaviorShotUpdate();

		break;
	}


	// 基底クラス更新
	BaseEnemy::Update();

	// モデルの更新
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		models_[i].worldTransform_->UpdateWorldMatrix();
		models_[i].uvTransform_->UpdateWorldMatrix();
	}

	// ポイントライトを本体に追従する
	pointLight_->position_ = GetBodyWorldPosition() + Vector3(0.0f , 0.5f , 0.0f);
}

/// <summary>
/// 描画処理
/// </summary>
void EnemyButterfly::Draw()
{
	// ポイントライトを設置する
	engine_->SetPointLight(pointLight_.get());

	// モデルの描画
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		engine_->DrawModel(models_[i].worldTransform_.get(), models_[i].uvTransform_.get(), camera3d_, models_[i].modelHandle_,
			models_[i].color, true);
	}

	// ダメージギミックの描画
	GimmickDamageDraw();
}

/// <summary>
/// 本体のワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 EnemyButterfly::GetBodyWorldPosition()
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = models_[kBody].worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = models_[kBody].worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = models_[kBody].worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}


/// <summary>
/// 衝突判定応答
/// </summary>
/// <param name="playerBullet"></param>
void EnemyButterfly::OnCollision(const BasePlayerBullet* playerBullet)
{
	// 基底クラスの衝突判定応答
	BaseEnemy::OnCollision(playerBullet);

	// ダメージギミック初期化
	GimmickDamageInitialize();
}



/*-----------------
    浮遊ギミック
-----------------*/

/// <summary>
/// ギミック : 浮遊 : 初期化
/// </summary>
void EnemyButterfly::GimmickFloatingInitialize()
{
	// 浮遊ギミックのパラメータ
	floatingParameter_ = 0.0f;

	// 浮遊ギミックの速度
	floatingVelocity_ = 0.075f;

	// 浮遊ギミックの振幅
	floatingAmplitude_ = 0.25f;
}

/// <summary>
/// ギミック : 浮遊 : 更新処理
/// </summary>
void EnemyButterfly::GimmickFloatingUpdate()
{
	// パラメータを進める
	floatingParameter_ += floatingVelocity_;
	floatingParameter_ = std::fmod(floatingParameter_, kFloatingParameterMax);

	// 上下に移動する
	models_[kBody].worldTransform_->translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;
}


/*-----------------------
	ギミック : 羽ばたく
-----------------------*/

/// <summary>
/// ギミック : 羽ばたく : 初期化
/// </summary>
void EnemyButterfly::GimmickFlappingInitialize()
{
	// 羽ばたきギミックのパラメータ
	flappingParameter_ = 0.0f;

	// 羽ばたきギミックの速度
	flappingVelocity_ = 0.085f;

	// 幅だきぎいっくの振幅
	flappingAmplitude_ = 0.75f;
}

/// <summary>
/// ギミック : 羽ばたく : 更新処理
/// </summary>
void EnemyButterfly::GimmickFlappingUpdate()
{
	// パラメータを進める
	flappingParameter_ += flappingVelocity_;
	flappingParameter_ = std::fmod(flappingParameter_, kFlappingPrameterMax);

	// 羽を動かす
	models_[kWingR].worldTransform_->rotation_.y = std::sin(flappingParameter_) * flappingAmplitude_;
	models_[kWingL].worldTransform_->rotation_.y = -std::sin(flappingParameter_) * flappingAmplitude_;
}


/*----------------------
	ギミック : ダメージ
----------------------*/

/// <summary>
/// ギミック : ダメージ : 初期化
/// </summary>
void EnemyButterfly::GimmickDamageInitialize()
{
	// ダメージギミックのパラメータ
	damageParameter_ = 0.0f;

	// ダメージの色
	damageColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

/// <summary>
/// ギミック : ダメージ : 更新処理
/// </summary>
void EnemyButterfly::GimmickDamageUpdate()
{
	// パラメータが最大値に達したら処理しない
	if (damageParameter_ >= kDamageParameterMax)
		return;

	// パラメータを進める
	damageParameter_ += damageVelocity_;
	damageParameter_ = std::min(damageParameter_, kDamageParameterMax);

	// 補間
	float t = damageParameter_ / kDamageParameterMax;

	// 徐々に透明にする
	damageColor.w = (1.0f - t);
}

/// <summary>
/// ギミック : ダメージ : 描画処理
/// </summary>
void EnemyButterfly::GimmickDamageDraw()
{
	// パラメータが最大値に達したら処理しない
	if (damageParameter_ >= kDamageParameterMax)
		return;

	// モデルの描画
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		engine_->DrawModel(models_[i].worldTransform_.get(), models_[i].uvTransform_.get(), camera3d_, models_[i].modelHandle_,
			damageColor, false);
	}
}


/*--------------------
    ギミック : 発射
--------------------*/

/// <summary>
/// ギミック : 発射動作 : 初期化
/// </summary>
void EnemyButterfly::GimmickShotActionInitliaze()
{
	// 発射動作パラメータ
	shotActionParameter_ = 0.0f;

	// 現在の角度
	shotActionCurrentRotation_ = worldTransform_->rotation_.y;

	// 発射フラグ
	isShot_ = false;
}

/// <summary>
/// ギミック : 発射動作 : 更新処理
/// </summary>
void EnemyButterfly::GimmickShotActionUpdate()
{
	// 発射ビヘイビアが終了するまでパラメータを進める
	if (shotActionParameter_ >= kShotParameterMax)
		return;

	// パラメータを進める
	shotActionParameter_ += kShotParameterVelocity;
	shotActionParameter_ = std::min(shotActionParameter_, kShotParameterMax);

	// 羽を広げる
	if (shotActionParameter_ >= kShotActionStartRotationParameter[0] && shotActionParameter_ <= kShotActionStartRotationParameter[1])
	{
		// 補間
		float t = (shotActionParameter_ - kShotActionStartRotationParameter[0]) / (kShotActionStartRotationParameter[1] - kShotActionStartRotationParameter[0]);
		float easing = 1.0f - std::powf(1.0f - t, 3);

		models_[kWingR].worldTransform_->rotation_.y = Lerp(models_[kWingR].worldTransform_->rotation_.y, kShotActionStartRotation[kWingR], t);
		models_[kWingL].worldTransform_->rotation_.y = Lerp(models_[kWingL].worldTransform_->rotation_.y, kShotActionStartRotation[kWingL], t);
	}
	
	// 羽を閉じる
	if (shotActionParameter_ >= kShotActionRotationParameter[0] && shotActionParameter_ <= kShotActionRotationParameter[1])
	{
		// 補間
		float t = (shotActionParameter_ - kShotActionRotationParameter[0]) / (kShotActionRotationParameter[1] - kShotActionRotationParameter[0]);
		float easing = 1.0f - std::powf(1.0f - t, 3);

		models_[kWingR].worldTransform_->rotation_.y = Lerp(kShotActionStartRotation[kWingR], kShotActionRotation[kWingR], t);
		models_[kWingL].worldTransform_->rotation_.y = Lerp(kShotActionStartRotation[kWingL], kShotActionRotation[kWingL], t);
	}

	// 羽を閉じ終えたら発射する
	if (shotActionParameter_ >= kShotActionRotationParameter[1])
	{
		if (isShot_ == false)
		{
			isShot_ = true;
			BulletShot();
		}
	}

	// 羽を戻す
	if (shotActionParameter_ >= kShotActionEndRotationParameter[0] && shotActionParameter_ <= kShotActionEndRotationParameter[1])
	{
		// 補間
		float t = (shotActionParameter_ - kShotActionEndRotationParameter[0]) / (kShotActionEndRotationParameter[1] - kShotActionEndRotationParameter[0]);

		models_[kWingR].worldTransform_->rotation_.y = Lerp(kShotActionRotation[kWingR], kShotActionEndRotation[kWingR], t);
		models_[kWingL].worldTransform_->rotation_.y = Lerp(kShotActionRotation[kWingL], kShotActionEndRotation[kWingL], t);
	}


	// ターゲットの方向を向くようにする
	Vector3 toTarget = -1.0f * Normalize(target_->GetBodyWorldPosition() - GetBodyWorldPosition());
	worldTransform_->rotation_.y = std::atan2(toTarget.x, toTarget.z);
	float length = std::sqrt(std::pow(toTarget.x, 2.0f) + std::pow(toTarget.z, 2.0f));
	worldTransform_->rotation_.x = std::atan2(-toTarget.y, length);
}



/*----------------------
    ビヘイビア : 通常
----------------------*/

/// <summary>
/// ビヘイビア : 通常 : 初期化
/// </summary>
void EnemyButterfly::BehaviorNormalInitialize()
{
	// 発射タイマー
	shotTimer_ = 0.0f;

	// 羽ばたきギミック初期化
	GimmickFlappingInitialize();
}

/// <summary>
/// ビヘイビア : 通常 : 更新処理
/// </summary>
void EnemyButterfly::BehaviorNormalUpdate()
{
	// タイマーを進める
	shotTimer_ += kShotTimerVelocity;
	shotTimer_ = std::min(shotTimer_, kShotTime);

	// 発射ビヘイビアに遷移する
	if (shotTimer_ >= kShotTime)
		requestBehavior_ = kShot;


	// 浮遊ギミック 更新
	GimmickFloatingUpdate();

	// 羽ばたきギミック 更新
	GimmickFlappingUpdate();

	// ダメージギミック 更新
	GimmickDamageUpdate();
}



/*----------------------
    ビヘイビア : 発射
----------------------*/

/// <summary>
/// ビヘイビア : 発射 : 初期化
/// </summary>
void EnemyButterfly::BehaviorShotInitialize()
{
	// 発射パラメータ
	shotParameter_ = 0.0f;

	// 発射動作ギミック初期化
	GimmickShotActionInitliaze();
}

/// <summary>
/// ビヘイビア : 発射 : 更新処理
/// </summary>
void EnemyButterfly::BehaviorShotUpdate()
{
	// 発射パラメータを進める
	shotParameter_ += kShotParameterVelocity;
	shotParameter_ = std::min(shotParameter_, kShotParameterMax);
	
	// 発射パラメータが終了したら、通所ビヘイビアに遷移する
	if (shotParameter_ >= kShotParameterMax)
		requestBehavior_ = kNormal;


	// 浮遊ギミック 更新
	GimmickFloatingUpdate();

	// 発射動作ギミック更新
	GimmickShotActionUpdate();

	// ダメージギミック 更新
	GimmickDamageUpdate();
}


/// <summary>
/// 弾を発射する
/// </summary>
void EnemyButterfly::BulletShot()
{
	// 新規の弾の生成
	EnemyBulletWeek* enemyBullet = new EnemyBulletWeek();
	enemyBullet->Initialize(engine_, camera3d_, GetBodyWorldPosition());

	// ターゲットの方向に発射する
	enemyBullet->SetDirection(Normalize(target_->GetBodyWorldPosition() - GetBodyWorldPosition()));

	// ゲームシーンのリストに追加する
	gameScene_->EnemyBulletShot(enemyBullet);
}