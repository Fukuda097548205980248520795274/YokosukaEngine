#include "PlayerBulletStrong.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void PlayerBulletStrong::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラス初期化
	BasePlayerBullet::Initialize(engine, camera3d, position);


	// 本体の生成と初期化
	bulletWorldTransform_ = std::make_unique<WorldTransform>();
	bulletWorldTransform_->Initialize();
	bulletWorldTransform_->SetParent(worldTransform_.get());
	bulletWorldTransform_->scale_ *= 3.0f;
	bulletUvTransform_ = std::make_unique<UvTransform>();
	bulletUvTransform_->Initialize();
	bulletModelHandle_ = engine_->LoadModelData("./Resources/Models/playerBullet/week", "week.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerBulletStrong::Update()
{
	// 寿命タイマーを進める
	lifeTimer_ += 1.0f / 60.0f;

	// 寿命を越えたら消滅する
	if (lifeTimer_ >= kLifeTime)
		isFinished_ = true;


	// 向きと速度で移動する
	worldTransform_->translation_ += direction_ * speed;

	// 基底クラス更新
	BasePlayerBullet::Update();


	// 本体の更新
	bulletWorldTransform_->UpdateWorldMatrix();
	bulletUvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerBulletStrong::Draw()
{
	// 基底クラス描画
	BasePlayerBullet::Draw();

	// 本体を描画する
	engine_->DrawModel(bulletWorldTransform_.get(), bulletUvTransform_.get(), camera3d_, bulletModelHandle_, Vector4(1.0f, 0.2f, 0.2f, 1.0f), false);
}