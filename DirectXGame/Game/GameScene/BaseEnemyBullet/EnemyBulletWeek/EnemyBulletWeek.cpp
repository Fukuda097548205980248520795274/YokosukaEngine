#include "EnemyBulletWeek.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void EnemyBulletWeek::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラスの初期化
	BaseEnemyBullet::Initialize(engine, camera3d, position);

	worldTransform_->scale_ *= 3.0f;

	// ワールドトランスフォームの生成
	bulletWorldTransform_ = std::make_unique<WorldTransform>();
	bulletWorldTransform_->Initialize();
	bulletWorldTransform_->SetParent(worldTransform_.get());

	// UVトランスフォームの生成
	bulletUvTransform_ = std::make_unique<UvTransform>();
	bulletUvTransform_->Initialize();

	// モデルを読み込む
	bulletModelHandle_ = engine_->LoadModelData("./Resources/Models/enemyBullet/week", "week.obj");

	// ポイントライトの生成
	bulletPointLight_ = std::make_unique<PointLight>();
	bulletPointLight_->Initialize();
	bulletPointLight_->color_ = Vector4(0.5f, 0.5f, 1.0f, 1.0f);
	bulletPointLight_->radius_ = 32.0f;

	// 攻撃力
	power_ = 2;
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyBulletWeek::Update()
{
	// 寿命タイマーを進める
	lifeTimer_ += 1.0f / 60.0f;

	// 寿命を越えたら消滅する
	if (lifeTimer_ >= kLifeTime)
		isFinished_ = true;


	// 向きと速度で移動する
	worldTransform_->translation_ += direction_ * speed;

	// 基底クラス更新
	BaseEnemyBullet::Update();


	// 本体の更新
	bulletWorldTransform_->UpdateWorldMatrix();
	bulletUvTransform_->UpdateWorldMatrix();

	// 弾の位置にポイントライトを配置する
	bulletPointLight_->position_ = GetBulletWorldPosition();
}

/// <summary>
/// 描画処理
/// </summary>
void EnemyBulletWeek::Draw()
{
	// ポイントライトを設置する
	engine_->SetPointLight(bulletPointLight_.get());

	// 本体を描画する
	engine_->DrawModel(bulletWorldTransform_.get(), bulletUvTransform_.get(), camera3d_, bulletModelHandle_, Vector4(0.5f, 0.5f, 1.0f, 1.0f), false);
}


/// <summary>
/// 本体のワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 EnemyBulletWeek::GetBulletWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = bulletWorldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = bulletWorldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = bulletWorldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}