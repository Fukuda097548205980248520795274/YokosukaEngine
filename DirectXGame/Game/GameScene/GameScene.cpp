#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// Scene更新
	Scene::Initialize(engine);


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// ワールドトラスフォーム
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_.y = radius_;

	// uvトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデル
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	if (engine_->GetKeyPress(DIK_A))
	{
		direction_ = kLeft;
	}
	else if (engine_->GetKeyPress(DIK_D))
	{
		direction_ = kRight;
	}


	// 向きテーブル
	float directionTable[2]
	{
		-1.0f,
		1.0f
	};

	ImGui::Begin("worldTransform");
	ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.1f);
	ImGui::End();

	// プレイヤー方向のベクトル
	Vector3 toPlayaer = Normalize(worldTransform_->translation_);

	if (engine_->GetKeyPress(DIK_SPACE))
	{
		radianY_ += 0.01f;
	}

	worldTransform_->rotation_.y = radianY_;


	worldTransform_->rotation_.x = std::asin(toPlayaer.z);
	if(worldTransform_->translation_.y <= 0.0f)
		worldTransform_->rotation_.x = std::numbers::pi_v<float> - std::asin(toPlayaer.z);

	if (direction_ == kLeft)
		worldTransform_->rotation_.x *= -1.0f;


	// 移動速度
	Vector3 velocity = Vector3(0.0f, 0.0f, 1.0f);

	// 回転行列
	Matrix4x4 rotateMatrix = MakeRotateMatrix(Vector3(worldTransform_->rotation_.x, worldTransform_->rotation_.y, worldTransform_->rotation_.z));

	velocity = Normalize(TransformNormal(velocity, rotateMatrix));

	if (engine_->GetKeyPress(DIK_S))
	{
		worldTransform_->translation_ += velocity / 2.0f;
	}

	ImGui::Text("%f,%f,%f", velocity.x, velocity.y, velocity.z);
	

	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// 平行光源の設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// 球を描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_.get(), modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}