#include "EnemyButterfly.h"
#include "../../Player/Player.h"
#include "../../../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void EnemyButterfly::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const Vector3& position, int32_t hp)
{
	// 基底クラス初期化
	BaseEnemy::Initialize(engine, camera3d, modelHandleStore ,position, hp);

	worldTransform_->scale_ *= 2.0f;

	hitSize_ = { 2.0f , 2.0f , 1.0f };

	// 胴体の初期化
	models_[kBody].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kBody].worldTransform_->Initialize();
	models_[kBody].worldTransform_->SetParent(worldTransform_.get());
	models_[kBody].worldTransform_->translation_ = kStartPosition[kBody];
	models_[kBody].worldTransform_->rotation_ = kStartRotation[kBody];
	models_[kBody].color = Vector4(0.5f, 0.0f, 0.0f, 1.0f);

	// 右の羽の初期化
	models_[kWingR].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kWingR].worldTransform_->Initialize();
	models_[kWingR].worldTransform_->SetParent(models_[kBody].worldTransform_.get());
	models_[kWingR].worldTransform_->translation_ = kStartPosition[kWingR];
	models_[kWingR].worldTransform_->rotation_ = kStartRotation[kWingR];
	models_[kWingR].color = Vector4(0.5f, 0.5f, 1.0f, 1.0f);

	// 左の羽の初期化
	models_[kWingL].worldTransform_ = std::make_unique<WorldTransform>();
	models_[kWingL].worldTransform_->Initialize();
	models_[kWingL].worldTransform_->SetParent(models_[kBody].worldTransform_.get());
	models_[kWingL].worldTransform_->translation_ = kStartPosition[kWingL];
	models_[kWingL].worldTransform_->rotation_ = kStartRotation[kWingL];
	models_[kWingL].color = Vector4(0.5f, 0.5f, 1.0f, 1.0f);

	// モデルハンドルを受け取る
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		models_[i].modelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kEnemyButterfly)[i];
	}


	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();


	state_ = std::make_unique<EnemyButterflyStateStop>(this);
}

/// <summary>
/// 更新処理
/// </summary>
void EnemyButterfly::Update()
{
	// ステート更新
	state_->Update();

	// 基底クラス更新
	BaseEnemy::Update();

	// モデルの更新
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		models_[i].worldTransform_->UpdateWorldMatrix();
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
		engine_->DrawModel(models_[i].worldTransform_.get(), camera3d_, models_[i].modelHandle_,models_[i].color, true);
	}

	// ダメージギミックの描画
	GimmickDamageDraw();
}

/// <summary>
/// 本体のワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 EnemyButterfly::GetBodyWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = models_[kBody].worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = models_[kBody].worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = models_[kBody].worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}

/// <summary>
/// 当たり判定用のAABBのGetter
/// </summary>
/// <returns></returns>
AABB EnemyButterfly::GetCollisionAABB() const
{
	// AABB
	AABB aabb;

	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_->scale_, worldTransform_->rotation_, worldTransform_->translation_) *
		MakeAffineMatrix(models_[kBody].worldTransform_->scale_, models_[kBody].worldTransform_->rotation_, models_[kBody].worldTransform_->translation_);

	aabb.max = hitSize_ + Vector3(worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2]);
	aabb.min = (-1.0f * hitSize_) + Vector3(worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2]);

	return aabb;
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

/// <summary>
/// ステートを変更
/// </summary>
/// <param name="state"></param>
void EnemyButterfly::ChangeState(std::unique_ptr<BaseEnemyButterflyState> state)
{
	state_ = std::move(state);
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
		engine_->DrawModel(models_[i].worldTransform_.get(), camera3d_, models_[i].modelHandle_, damageColor, false);
	}
}


/// <summary>
/// 弾を発射する
/// </summary>
void EnemyButterfly::BulletShot()
{
	// 新規の弾の生成
	std::unique_ptr<EnemyBulletWeek> enemyBullet = std::make_unique<EnemyBulletWeek>();
	enemyBullet->Initialize(engine_, camera3d_, modelHandleStore_ , worldTransform_->translation_);

	// ターゲットの方向に発射する
	enemyBullet->SetDirection(Normalize(target_->GetWorldTransform()->translation_ - worldTransform_->translation_));

	// ゲームシーンのリストに追加する
	gameScene_->EnemyBulletShot(std::move(enemyBullet));
}