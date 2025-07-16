#include "Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Player::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// 基底クラス初期化
	BaseCharacter::Initialize(engine, camera3d);


	// 胴体
	models_[kBody].worldTransform = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform->Initialize();
	models_[kBody].worldTransform->translation_ = modelsStartPosition[kBody];
	models_[kBody].worldTransform->SetParent(worldTransform_.get());
	models_[kBody].uvTransform = std::make_unique<UvTransform>();
	models_[kBody].uvTransform->Initialize();
	models_[kBody].modelHandle = engine_->LoadModelData("./Resources/Models/Player/body", "body.obj");
	models_[kBody].color = Vector4(1.0f, 1.0f, 0.4f, 1.0f);

	// 頭
	models_[kHead].worldTransform = std::make_unique<WorldTransform>();
	models_[kHead].worldTransform->Initialize();
	models_[kHead].worldTransform->translation_ = modelsStartPosition[kHead];
	models_[kHead].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kHead].uvTransform = std::make_unique<UvTransform>();
	models_[kHead].uvTransform->Initialize();
	models_[kHead].modelHandle = engine_->LoadModelData("./Resources/Models/Player/head", "head.obj");
	models_[kHead].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	// 左腕
	models_[kLArm].worldTransform = std::make_unique<WorldTransform>();
	models_[kLArm].worldTransform->Initialize();
	models_[kLArm].worldTransform->translation_ = modelsStartPosition[kLArm];
	models_[kLArm].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kLArm].uvTransform = std::make_unique<UvTransform>();
	models_[kLArm].uvTransform->Initialize();
	models_[kLArm].modelHandle = engine_->LoadModelData("./Resources/Models/Player/L_arm", "L_arm.obj");
	models_[kLArm].color = Vector4(0.4f, 0.4f, 1.0f, 1.0f);

	// 右腕
	models_[kRArm].worldTransform = std::make_unique<WorldTransform>();
	models_[kRArm].worldTransform->Initialize();
	models_[kRArm].worldTransform->translation_ = modelsStartPosition[kRArm];
	models_[kRArm].worldTransform->SetParent(models_[kBody].worldTransform.get());
	models_[kRArm].uvTransform = std::make_unique<UvTransform>();
	models_[kRArm].uvTransform->Initialize();
	models_[kRArm].modelHandle = engine_->LoadModelData("./Resources/Models/Player/R_arm", "R_arm.obj");
	models_[kRArm].color = Vector4(1.0f, 0.4f, 0.4f, 1.0f);

	// 浮遊ギミックの初期化
	InitializeFloatingGimmick();

	// 手揺れギミックの初期化
	InitializeHandSwingGimmick();
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &models_[kHead].worldTransform->translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("LArm Translation", &models_[kLArm].worldTransform->translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("RArm Translation", &models_[kRArm].worldTransform->translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat("floating time", &floatingTime_, 0.1f, 3.0f);
	ImGui::SliderFloat("floating amplitude", &floatingAmplitude_, 0.0f, 3.0f);
	ImGui::SliderFloat("handSwing time", &handSwingTime_, 0.1f, 3.0f);
	ImGui::SliderFloat("handSwing amplitude", &handSwingAmplitude_, 0.0f, 3.0f);
	ImGui::End();


	// 入力操作
	Input();

	// 浮遊ギミックの更新処理
	UpdateFloatingGimmick();

	// 手揺れギミックの更新処理
	UpdateHandSwingGimmick();

	// 基底クラス更新
	BaseCharacter::Update();

	// モデルを更新する
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		models_[i].worldTransform->UpdateWorldMatrix();
		models_[i].uvTransform->UpdateWorldMatrix();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// モデルを描画する
	for (uint32_t i = 0; i < kNumModels; i++)
	{
		engine_->DrawModel(models_[i].worldTransform.get(), models_[i].uvTransform.get(), camera3d_,
			models_[i].modelHandle, models_[i].color, false);
	}
}


/// <summary>
/// 入力操作
/// </summary>
void Player::Input()
{
	// 移動操作
	Move();
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
/// 浮遊ギミック初期化
/// </summary>
void Player::InitializeFloatingGimmick()
{
	floatingTime_ = 1.0f;
	floatingParameter_ = 0.0f;
	floatingAmplitude_ = 0.1f;
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