#include "PlayerBulletWeek.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void PlayerBulletWeek::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, WorldTransform* parent)
{
	// 基底クラス初期化
	BasePlayerBullet::Initialize(engine, camera3d, position , parent);


	// 本体の生成と初期化
	bulletWorldTransform_ = std::make_unique<WorldTransform>();
	bulletWorldTransform_->Initialize();
	bulletWorldTransform_->SetParent(worldTransform_.get());
	bulletModelHandle_ = engine_->LoadModelData("./Resources/Models/playerBullet/week", "week.obj");

	// 弾のポイントライトの生成と初期化
	bulletPointLight_ = std::make_unique<PointLight>();
	bulletPointLight_->Initialize();
	bulletPointLight_->color_ = Vector4(0.2f, 1.0f, 0.2f, 1.0f);
	bulletPointLight_->radius_ = 16.0f;

	// 攻撃力
	power_ = 2;
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerBulletWeek::Update()
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

	// 弾の位置にポイントライトを配置する
	bulletPointLight_->position_ = GetBulletWorldTransform();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerBulletWeek::Draw()
{
	// 基底クラス描画
	BasePlayerBullet::Draw();

	// ポイントライトを設置する
	engine_->SetPointLight(bulletPointLight_.get());

	// 本体を描画する
	engine_->DrawModel(bulletWorldTransform_.get(), camera3d_, bulletModelHandle_, Vector4(0.2f, 1.0f, 0.2f, 1.0f), false);
}

/// <summary>
/// 弾本体のワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 PlayerBulletWeek::GetBulletWorldTransform()
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = bulletWorldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = bulletWorldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = bulletWorldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}