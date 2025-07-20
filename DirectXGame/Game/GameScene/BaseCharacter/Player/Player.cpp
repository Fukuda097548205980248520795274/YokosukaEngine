#include "Player.h"

const std::array<Player::ConstAttack, Player::ComboNum> Player::kConstAttacks =
{
	{
	{0.0f , 0.0f , 0.3f , 0.3f , 0.0f , 0.0f , 1.0f},
	{0.2f , 0.15f , 0.05f , 0.3f , 0.0f , 0.0f , 3.0f},
	{0.0f , 0.15f , 0.3f , 0.3f , 0.0f , 0.0f , 1.0f}
	}
};

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// 基底クラス初期化
	BaseCharacter::Initialize(engine, camera3d);

	// 調整項目クラス
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	// 胴体
	models_[kBody].worldTransform = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform->Initialize();
	models_[kBody].worldTransform->translation_ = modelsStartPosition[kBody];
	models_[kBody].worldTransform->SetParent(worldTransform_.get());
	models_[kBody].modelHandle = engine_->LoadModelData("./Resources/Models/Player/body", "body.obj");
	models_[kBody].color = Vector4(1.0f, 1.0f, 0.4f, 1.0f);

	// 頭
	models_[kHead].worldTransform = std::make_unique<WorldTransform>();
	models_[kHead].worldTransform->Initialize();
	models_[kHead].worldTransform->translation_ = modelsStartPosition[kHead];
	models_[kHead].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kHead].modelHandle = engine_->LoadModelData("./Resources/Models/Player/head", "head.obj");
	models_[kHead].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	// 左腕
	models_[kLArm].worldTransform = std::make_unique<WorldTransform>();
	models_[kLArm].worldTransform->Initialize();
	models_[kLArm].worldTransform->translation_ = modelsStartPosition[kLArm];
	models_[kLArm].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kLArm].modelHandle = engine_->LoadModelData("./Resources/Models/Player/L_arm", "L_arm.obj");
	models_[kLArm].color = Vector4(0.4f, 0.4f, 1.0f, 1.0f);

	// 右腕
	models_[kRArm].worldTransform = std::make_unique<WorldTransform>();
	models_[kRArm].worldTransform->Initialize();
	models_[kRArm].worldTransform->translation_ = modelsStartPosition[kRArm];
	models_[kRArm].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kRArm].modelHandle = engine_->LoadModelData("./Resources/Models/Player/R_arm", "R_arm.obj");
	models_[kRArm].color = Vector4(1.0f, 0.4f, 0.4f, 1.0f);

	// 武器
	models_[kWeapon].worldTransform = std::make_unique<WorldTransform>();
	models_[kWeapon].worldTransform->Initialize();
	models_[kWeapon].worldTransform->translation_ = modelsStartPosition[kWeapon];
	models_[kWeapon].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kWeapon].modelHandle = engine_->LoadModelData("./Resources/Models/Player/weapon", "weapon.obj");
	models_[kWeapon].color = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	for (uint32_t i = 0; i < kNumModels; i++)
	{
		// UVトランスフォームの生成と初期化
		models_[i].uvTransform = std::make_unique<UvTransform>();
		models_[i].uvTransform->Initialize();

		// 影の生成
		models_[i].planeProjectionShadow = std::make_unique<PlaneProjectionShadow>();
		models_[i].planeProjectionShadow->Initialize(engine_, camera3d_, models_[i].modelHandle, models_[i].worldTransform.get());
	}

	// 浮遊ギミックの初期化
	InitializeFloatingGimmick();

	// 手揺れギミックの初期化
	InitializeHandSwingGimmick();

	globalVariables->AddItem(groupName, "Head Translation", models_[kHead].worldTransform->translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", models_[kLArm].worldTransform->translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", models_[kRArm].worldTransform->translation_);
	globalVariables->AddItem(groupName, "floating time", floatingTime_);
	globalVariables->AddItem(groupName, "floating Amplitude", floatingAmplitude_);
	globalVariables->AddItem(groupName, "handSwing Amplitude", handSwingAmplitude_);
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	// 調整項目の適用
	ApplyGlobalVaribles();

	// 遷移処理
	if (behaviorRequest_)
	{
		// ふるまいを変更する
		behavior_ = behaviorRequest_.value();

		// 初期化
		switch (behavior_)
		{
		case Behavior::kRoot:
		default:

			// 通常
			BehaviorRootInitialize();

			break;

		case Behavior::kAttack:

			// 攻撃
			BehaviorAttackInitialize();

			break;

		case Behavior::kDash:

			// ダッシュ
			BehaviorDashInitialize();

			break;

		case Behavior::kJump:

			// ジャンプ
			BehaviorJumpInitialize();

			break;
		}

		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// ふるまい更新処理
	switch (behavior_)
	{
	case Behavior::kRoot:
	default:

		// 通常
		BehaviorRootUpdate();

		break;

	case Behavior::kAttack:

		// 攻撃
		BehaviorAttackUpdate();

		break;

	case Behavior::kDash:

		// ダッシュ
		BehaviorDashUpdate();

		break;

	case Behavior::kJump:

		// ジャンプ
		BehaviorJumpUpdate();

		break;
	}
	

	// 基底クラス更新
	BaseCharacter::Update();

	// モデルを更新する
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		models_[i].worldTransform->UpdateWorldMatrix();
		models_[i].uvTransform->UpdateWorldMatrix(); 

		// 影
		models_[i].planeProjectionShadow->Update();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// ふるまい描画処理
	switch (behavior_)
	{
	case Behavior::kRoot:
	default:

		// 通常
		BehaviorRootDraw();

		break;

	case Behavior::kAttack:

		// 攻撃
		BehaviorAttackDraw();

		break;

	case Behavior::kDash:

		// ダッシュ
		BehaviorDashDraw();

		break;

	case Behavior::kJump:

		// ジャンプ
		BehaviorJumpDraw();

		break;
	}
}


/// <summary>
/// 調整項目の適用
/// </summary>
void Player::ApplyGlobalVaribles()
{
	// 調整項目クラス
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	models_[kHead].worldTransform->translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
	models_[kLArm].worldTransform->translation_ = globalVariables->GetVector3Value(groupName, "ArmL Translation");
	models_[kRArm].worldTransform->translation_ = globalVariables->GetVector3Value(groupName, "ArmR Translation");
	floatingTime_ = globalVariables->GetFloatValue(groupName, "floating time");
	floatingAmplitude_ = globalVariables->GetFloatValue(groupName, "floating Amplitude");
	handSwingAmplitude_ = globalVariables->GetFloatValue(groupName, "handSwing Amplitude");
}

/// <summary>
/// 入力操作
/// </summary>
void Player::Input()
{
	// 移動操作
	Move();

	// 攻撃操作
	Attack();

	// ダッシュ操作
	Dash();

	// ジャンプ操作
	Jump();
}

/// <summary>
/// 移動操作
/// </summary>
void Player::Move()
{
	// コントローラが使えるかどうか
	if (engine_->IsGamepadEnable(0))
	{
		// デッドゾーン
		const float kDeadZone = 0.7f;

		// 移動フラグ
		bool isMove = false;

		// 移動量
		Vector3 move =
		{
			engine_->GetGamepadLeftStick(0).x ,
			0.0f,
			engine_->GetGamepadLeftStick(0).y
		};

		// デッドゾーンを越えないときは処理しない
		if (Length(move) >= kDeadZone)
		{
			isMove = true;
		}


		// 移動しているときの処理
		if (isMove)
		{
			// 早さ
			const float speed = 0.3f;

			// 移動量に早さを反映する
			move = Normalize(move) * speed;

			// メインカメラの向きで移動方向を変える
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(mainCamera_->rotation_.y);
			move = TransformNormal(move, rotateMatrix);

			// 移動させる
			worldTransform_->translation_ += move;

			// 移動方向に合わせて回転する目標角度
			toRotationY_ = std::atan2(move.x, move.z);
		}
	}

	// 最短角度補間
	worldTransform_->rotation_.y = LerpShortAngle(worldTransform_->rotation_.y, toRotationY_, 0.2f);
}

/// <summary>
/// 攻撃操作
/// </summary>
void Player::Attack()
{
	// ゲームパッドが有効かどうか
	if (engine_->IsGamepadEnable(0) == false)
		return;

	// Aボタンで攻撃
	if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_A))
	{
		behaviorRequest_ = Behavior::kAttack;
	}
}

/// <summary>
/// ダッシュ操作
/// </summary>
void Player::Dash()
{
	// ゲームパッドが有効かどうか
	if (engine_->IsGamepadEnable(0) == false)
		return;

	// Bボタンでダッシュ
	if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_B))
	{
		behaviorRequest_ = Behavior::kDash;
	}
}

/// <summary>
/// ジャンプ操作
/// </summary>
void Player::Jump()
{
	// ゲームパッドが有効かどうか
	if (engine_->IsGamepadEnable(0) == false)
		return;

	// Xボタンでダッシュ
	if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_X))
	{
		behaviorRequest_ = Behavior::kJump;
	}
}



/*--------------------
    ふるまい : 通常
--------------------*/

/// <summary>
/// ふるまい : 通常 : 初期化
/// </summary>
void Player::BehaviorRootInitialize()
{
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		models_[i].worldTransform->translation_ = modelsStartPosition[i];
		models_[i].worldTransform->rotation_ = modelsStartRotation[i];
	}
}

/// <summary>
/// ふるまい : 通常 : 更新処理
/// </summary>
void Player::BehaviorRootUpdate()
{
	// 入力操作
	Input();

	// 浮遊ギミックの更新処理
	UpdateFloatingGimmick();

	// 手揺れギミックの更新処理
	UpdateHandSwingGimmick();
}

/// <summary>
/// ふるまい : 通常 : 描画処理
/// </summary>
void Player::BehaviorRootDraw()
{
	models_[kBody].planeProjectionShadow->Draw();
	models_[kHead].planeProjectionShadow->Draw();
	models_[kRArm].planeProjectionShadow->Draw();
	models_[kLArm].planeProjectionShadow->Draw();

	engine_->DrawModel(models_[kBody].worldTransform.get(), models_[kBody].uvTransform.get(), camera3d_,
		models_[kBody].modelHandle, models_[kBody].color, false);

	engine_->DrawModel(models_[kHead].worldTransform.get(), models_[kHead].uvTransform.get(), camera3d_,
		models_[kHead].modelHandle, models_[kHead].color, false);

	engine_->DrawModel(models_[kRArm].worldTransform.get(), models_[kRArm].uvTransform.get(), camera3d_,
		models_[kRArm].modelHandle, models_[kRArm].color, false);

	engine_->DrawModel(models_[kLArm].worldTransform.get(), models_[kLArm].uvTransform.get(), camera3d_,
		models_[kLArm].modelHandle, models_[kLArm].color, false);
}


/*--------------------
    ふるまい : 攻撃
--------------------*/

/// <summary>
/// ふるまい : 攻撃 : 初期化
/// </summary>
void Player::BehaviorAttackInitialize()
{
	// 旋回に補間をかけない
	worldTransform_->rotation_.y = toRotationY_;

	// モデルの回転
	models_[kRArm].worldTransform->rotation_.x = -std::numbers::pi_v<float> / 2.0f;
	models_[kLArm].worldTransform->rotation_.x = -std::numbers::pi_v<float> / 2.0f;
	models_[kWeapon].worldTransform->rotation_.x = std::numbers::pi_v<float> / 2.0f;

	// 攻撃ワークの初期化
	workAttack_.parameter_ = 0.0f;
	workAttack_.comboIndex = 0;
	workAttack_.inComboPhase = 0;
	workAttack_.comboNext = false;
}

/// <summary>
/// ふるまい : 攻撃 : 更新処理
/// </summary>
void Player::BehaviorAttackUpdate()
{
	// 予備動作の時間
	const float kAnticipationTime = kConstAttacks[workAttack_.comboIndex].anticipationTime;
	const float kChargeTime = kConstAttacks[workAttack_.comboIndex].chargeTime;
	const float kSwingTime = kConstAttacks[workAttack_.comboIndex].swingTime;
	const float kRecoveryTime = kConstAttacks[workAttack_.comboIndex].recoveryTime;

	// コンボの段階によってモーションを分岐
	switch (workAttack_.inComboPhase)
	{
	case 0:
		// 右から反時計回り

		// 振りかぶり
		if (workAttack_.parameter_ <= kAnticipationTime)
		{
			break;
		}

		// 溜め
		if (workAttack_.parameter_ <= kAnticipationTime + kChargeTime)
		{
			break;
		}

		// 攻撃振り
		if (workAttack_.parameter_ <= kAnticipationTime + kChargeTime + kSwingTime)
		{
			// 補間
			float t = (workAttack_.parameter_ - kAnticipationTime + kChargeTime) / kSwingTime;

			const float radian = -(std::numbers::pi_v<float> *2.0f);
			models_[kBody].worldTransform->rotation_.y = Lerp(0.0f, radian, t);

			// 向いている方向に進む
			Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->rotation_);
			Vector3 velocity = Normalize(Transform(Vector3(0.0f, 0.0f, 1.0f), rotateMatrix)) * kConstAttacks[workAttack_.comboIndex].swingSpeed;
			worldTransform_->translation_ += velocity;

			break;
		}

		break;

	case 1:
		// 上から振り下ろし

		// 振りかぶり
		if (workAttack_.parameter_ <= kAnticipationTime)
		{
			// 補間
			float t = workAttack_.parameter_ / kAnticipationTime;

			const float stepRadian = (std::numbers::pi_v<float> / 2.0f) / (60.0f * kAnticipationTime);

			models_[kRArm].worldTransform->rotation_.x -= stepRadian;
			models_[kLArm].worldTransform->rotation_.x -= stepRadian;
			models_[kWeapon].worldTransform->rotation_.x -= stepRadian;

			break;
		}

		// 溜め
		if (workAttack_.parameter_ <= kAnticipationTime + kChargeTime)
		{
			// 補間
			float t = (workAttack_.parameter_ - kAnticipationTime) / kChargeTime;

			break;
		}

		// 攻撃振り
		if (workAttack_.parameter_ <= kAnticipationTime + kChargeTime + kSwingTime)
		{
			// 補間
			float t = (workAttack_.parameter_ - kAnticipationTime + kChargeTime) / kSwingTime;
			const float stepRadian = (std::numbers::pi_v<float> / 2.0f) / (60.0f * kSwingTime);

			models_[kRArm].worldTransform->rotation_.x += stepRadian;
			models_[kLArm].worldTransform->rotation_.x += stepRadian;
			models_[kWeapon].worldTransform->rotation_.x += stepRadian;

			// 向いている方向に進む
			Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->rotation_);
			Vector3 velocity = Normalize(Transform(Vector3(0.0f, 0.0f, 1.0f), rotateMatrix)) * kConstAttacks[workAttack_.comboIndex].swingSpeed;
			worldTransform_->translation_ += velocity;

			break;
		}

		break;

	case 2:
	default:
		// 右からホームラン

		// 振りかぶり
		if (workAttack_.parameter_ <= kAnticipationTime)
		{
			break;
		}

		// 溜め
		if (workAttack_.parameter_ <= kAnticipationTime + kChargeTime)
		{
			break;
		}

		// 攻撃振り
		if (workAttack_.parameter_ <= kAnticipationTime + kChargeTime + kSwingTime)
		{
			// 補間
			float t = (workAttack_.parameter_ - kAnticipationTime + kChargeTime) / kSwingTime;

			const float radian = -(std::numbers::pi_v<float> *2.0f);
			models_[kBody].worldTransform->rotation_.y = Lerp(0.0f, radian, t);

			// 向いている方向に進む
			Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->rotation_);
			Vector3 velocity = Normalize(Transform(Vector3(0.0f, 0.0f, 1.0f), rotateMatrix)) * kConstAttacks[workAttack_.comboIndex].swingSpeed;
			worldTransform_->translation_ += velocity;

			break;
		}

		break;
	}

	// パラメータを進める
	workAttack_.parameter_ += 1.0f / 60.0f;

	// コンボ上限に達していない
	if (workAttack_.comboIndex < ComboNum - 1)
	{
		// ゲームパッドが有効のときのみ
		if (engine_->IsGamepadEnable(0))
		{
			// Aボタンで攻撃
			if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_A))
			{
				workAttack_.comboNext = true;
			}
		}
	}

	// 既定の時間経過で通常行動に戻る
	if (workAttack_.parameter_ >= kAnticipationTime + kChargeTime + kSwingTime + kRecoveryTime)
	{
		// コンボ継続なら次のコンボに進む
		if (workAttack_.comboNext)
		{
			// コンボ継続フラグをリセット
			workAttack_.comboNext = false;

			// 次の攻撃フェーズに移行する
			workAttack_.inComboPhase++;
			workAttack_.comboIndex++;

			// パラメータを初期化
			workAttack_.parameter_ = 0.0f;
		}
		else
		{
			// コンボが継続しないのなら、通常ビヘイビアに遷移
			behaviorRequest_ = Behavior::kRoot;
		}
	}
}

/// <summary>
/// ふるまい : 攻撃 : 描画処理
/// </summary>
void Player::BehaviorAttackDraw()
{
	models_[kBody].planeProjectionShadow->Draw();
	models_[kHead].planeProjectionShadow->Draw();
	models_[kRArm].planeProjectionShadow->Draw();
	models_[kLArm].planeProjectionShadow->Draw();
	models_[kWeapon].planeProjectionShadow->Draw();

	engine_->DrawModel(models_[kBody].worldTransform.get(), models_[kBody].uvTransform.get(), camera3d_,
		models_[kBody].modelHandle, models_[kBody].color, false);

	engine_->DrawModel(models_[kHead].worldTransform.get(), models_[kHead].uvTransform.get(), camera3d_,
		models_[kHead].modelHandle, models_[kHead].color, false);

	engine_->DrawModel(models_[kRArm].worldTransform.get(), models_[kRArm].uvTransform.get(), camera3d_,
		models_[kRArm].modelHandle, models_[kRArm].color, false);

	engine_->DrawModel(models_[kLArm].worldTransform.get(), models_[kLArm].uvTransform.get(), camera3d_,
		models_[kLArm].modelHandle, models_[kLArm].color, false);

	engine_->DrawModel(models_[kWeapon].worldTransform.get(), models_[kWeapon].uvTransform.get(), camera3d_,
		models_[kWeapon].modelHandle, models_[kWeapon].color, false);
}


/*-----------------------
	ふるまい : ダッシュ
-----------------------*/

/// <summary>
/// ふるまい : ダッシュ : 初期化
/// </summary>
void Player::BehaviorDashInitialize()
{
	// パラメータ
	workDash_.parameter = 0.0f;

	// 速度
	workDash_.speed = 1.0f;

	// 旋回に補間をかけない
	worldTransform_->rotation_.y = toRotationY_;
}

/// <summary>
/// ふるまい : ダッシュ : 更新処理
/// </summary>
void Player::BehaviorDashUpdate()
{
	// 移動操作
	Move();

	// 向いている方向に進む
	Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->rotation_);
	Vector3 velocity = Normalize(Transform(Vector3(0.0f, 0.0f, 1.0f), rotateMatrix)) * workDash_.speed;

	// 移動する
	worldTransform_->translation_ += velocity;


	// パラメータを進める
	workDash_.parameter += 1.0f / 60.0f;
	workDash_.parameter = std::min(workDash_.parameter, workDash_.kPrameterMax);

	// 最大値を越えたら通常ビヘイビアに遷移
	if (workDash_.parameter >= workDash_.kPrameterMax)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

/// <summary>
/// ふるまい : ダッシュ : 描画処理
/// </summary>
void Player::BehaviorDashDraw()
{
	models_[kBody].planeProjectionShadow->Draw();
	models_[kHead].planeProjectionShadow->Draw();
	models_[kRArm].planeProjectionShadow->Draw();
	models_[kLArm].planeProjectionShadow->Draw();

	engine_->DrawModel(models_[kBody].worldTransform.get(), models_[kBody].uvTransform.get(), camera3d_,
		models_[kBody].modelHandle, models_[kBody].color, false);

	engine_->DrawModel(models_[kHead].worldTransform.get(), models_[kHead].uvTransform.get(), camera3d_,
		models_[kHead].modelHandle, models_[kHead].color, false);

	engine_->DrawModel(models_[kRArm].worldTransform.get(), models_[kRArm].uvTransform.get(), camera3d_,
		models_[kRArm].modelHandle, models_[kRArm].color, false);

	engine_->DrawModel(models_[kLArm].worldTransform.get(), models_[kLArm].uvTransform.get(), camera3d_,
		models_[kLArm].modelHandle, models_[kLArm].color, false);
}



/*-----------------------
    ふるまい : ジャンプ
-----------------------*/

/// <summary>
/// ふるまい : ジャンプ : 初期化
/// </summary>
void Player::BehaviorJumpInitialize()
{
	models_[kBody].worldTransform->translation_.y = 0.0f;
	models_[kLArm].worldTransform->rotation_.x = 0.0f;
	models_[kRArm].worldTransform->rotation_.x = 0.0f;

	// 旋回に補間をかけない
	worldTransform_->rotation_.y = toRotationY_;

	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;

	// ジャンプの初速を与える
	velocity_.y = kJumpFirstSpeed;


	if (engine_->IsGamepadEnable(0))
	{
		Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->rotation_);
		Vector3 direction = Normalize(
			Transform(Vector3(0.0f, 0.0f, std::fabs(engine_->GetGamepadLeftStick(0).y)), rotateMatrix));

		velocity_.x = direction.x;
		velocity_.z = direction.z;
	}
}

/// <summary>
/// ふるまい : ジャンプ : 更新処理
/// </summary>
void Player::BehaviorJumpUpdate()
{
	// 移動
	worldTransform_->translation_ += velocity_;


	// 重力加速度
	const float kGravityAcceleration = 0.05f;

	// 加速度ベクトル
	Vector3 accelerationVector = { 0.0f , -kGravityAcceleration , 0.0f };

	// 加速する
	velocity_ += accelerationVector;


	// 着地したら通常ビヘイビアに遷移する
	if (worldTransform_->translation_.y <= 0.0f)
	{
		worldTransform_->translation_.y = 0.0f;
		behaviorRequest_ = Behavior::kRoot;
	}
}

/// <summary>
/// ふるまい : ジャンプ : 描画処理
/// </summary>
void Player::BehaviorJumpDraw()
{
	models_[kBody].planeProjectionShadow->Draw();
	models_[kHead].planeProjectionShadow->Draw();
	models_[kRArm].planeProjectionShadow->Draw();
	models_[kLArm].planeProjectionShadow->Draw();

	engine_->DrawModel(models_[kBody].worldTransform.get(), models_[kBody].uvTransform.get(), camera3d_,
		models_[kBody].modelHandle, models_[kBody].color, false);

	engine_->DrawModel(models_[kHead].worldTransform.get(), models_[kHead].uvTransform.get(), camera3d_,
		models_[kHead].modelHandle, models_[kHead].color, false);

	engine_->DrawModel(models_[kRArm].worldTransform.get(), models_[kRArm].uvTransform.get(), camera3d_,
		models_[kRArm].modelHandle, models_[kRArm].color, false);

	engine_->DrawModel(models_[kLArm].worldTransform.get(), models_[kLArm].uvTransform.get(), camera3d_,
		models_[kLArm].modelHandle, models_[kLArm].color, false);
}



/*--------------------
    ギミック : 浮遊
--------------------*/

/// <summary>
/// 浮遊ギミック初期化
/// </summary>
void Player::InitializeFloatingGimmick()
{
	floatingTime_ = 1.0f;
	floatingParameter_ = 0.0f;
	floatingAmplitude_ = 0.05f;
}

/// <summary>
/// 浮遊ギミック更新処理
/// </summary>
void Player::UpdateFloatingGimmick()
{
	// 1フレームでのパラメータ加算
	const float step = 2.0f * std::numbers::pi_v<float> / (floatingTime_ * 60.0f);


	// パラメータを1ステップ分加算
	floatingParameter_ += step;

	// 2πを越えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, std::numbers::pi_v<float> *2.0f);



	// 浮遊の振れ幅を座標に反映する
	models_[kBody].worldTransform->translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;
}


/*---------------------
    ギミック : 手揺れ
---------------------*/

/// <summary>
/// 手揺れギミック初期化
/// </summary>
void Player::InitializeHandSwingGimmick()
{
	handSwingTime_ = 2.0f;
	handSwingParameter_ = 0.0f;
	handSwingAmplitude_ = 0.5f;
}

/// <summary>
/// 手揺れギミック更新処理
/// </summary>
void Player::UpdateHandSwingGimmick()
{
	// 1フレームでのパラメータ加算
	const float step = 2.0f * std::numbers::pi_v<float> / (handSwingTime_ * 60.0f);

	// パラメータを1ステップ分加算
	handSwingParameter_ += step;

	// 2πを越えたら0に戻す
	handSwingParameter_ = std::fmod(handSwingParameter_, std::numbers::pi_v<float> *2.0f);

	// 手を揺らす
	models_[kLArm].worldTransform->rotation_.x = std::cos(handSwingParameter_) * handSwingAmplitude_;
	models_[kRArm].worldTransform->rotation_.x = std::cos(handSwingParameter_) * handSwingAmplitude_;
}