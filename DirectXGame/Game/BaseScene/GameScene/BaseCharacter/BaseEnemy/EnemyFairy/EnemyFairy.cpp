#include "EnemyFairy.h"
#include "../../Player/Player.h"
#include "../../../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void EnemyFairy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const Vector3& position, int32_t hp)
{
	// 基底クラス初期化
	BaseEnemy::Initialize(engine, camera3d, modelHandleStore, position, hp);

	hitSize_ = { 2.0f , 2.0f , 1.0f };


	// 共通した処理
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		// ワールドトラスフォームの生成と初期化
		models_[i].worldTransform_ = std::make_unique<WorldTransform>();
		models_[i].worldTransform_->Initialize();
		models_[i].worldTransform_->translation_ = kStartPosition[i];
		models_[i].worldTransform_->rotation_ = kStartRotation[i];

		// モデルハンドルを読み込む
		models_[i].modelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kEnemyFairy)[i];

		// ダメージカラーギミックの生成と初期化
		models_[i].gimmickDamageColor_ = std::make_unique<GimmickDamageColor>();
		models_[i].gimmickDamageColor_->Initialize(models_[i].worldTransform_.get(), 1.0f / 3.0f);
	}

	// 胴体の親を設定
	models_[kBody].worldTransform_->SetParent(worldTransform_.get());

	// 頭の親を設定
	models_[kHead].worldTransform_->SetParent(models_[kBody].worldTransform_.get());

	// 右腕の親を設定
	models_[kArmR].worldTransform_->SetParent(models_[kBody].worldTransform_.get());

	// 左腕の親を設定
	models_[kArmL].worldTransform_->SetParent(models_[kBody].worldTransform_.get());

	// 右足の親を設定
	models_[kLegR].worldTransform_->SetParent(models_[kBody].worldTransform_.get());

	// 左足の親を設定
	models_[kLegL].worldTransform_->SetParent(models_[kBody].worldTransform_.get());

	// 右の羽の親を設定
	models_[kWingR].worldTransform_->SetParent(models_[kBody].worldTransform_.get());

	// 左の羽の親を設定
	models_[kWingL].worldTransform_->SetParent(models_[kBody].worldTransform_.get());

	// 武器の親を設定
	models_[kWeapon].worldTransform_->SetParent(models_[kArmR].worldTransform_.get());


	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();

	// ステートの生成
	state_ = std::make_unique<EnemyFairyStateApproachingRear>(this);
}


/// <summary>
/// 更新処理
/// </summary>
void EnemyFairy::Update()
{
	// ステート更新
	state_->Update();

	// 基底クラス更新
	BaseEnemy::Update();

	// モデルの更新
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		models_[i].worldTransform_->UpdateWorldMatrix();

		// ダメージカラーギミックの更新
		models_[i].gimmickDamageColor_->Update();
	}

	// ポイントライトを本体に追従する
	pointLight_->position_ = GetBodyWorldPosition() + Vector3(0.0f, 0.5f, 0.0f);
}

/// <summary>
/// 描画処理
/// </summary>
void EnemyFairy::Draw()
{
	// ポイントライトを設置する
	engine_->SetPointLight(pointLight_.get());

	// モデルの描画
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		engine_->DrawModel(models_[i].worldTransform_.get(), camera3d_, models_[i].modelHandle_, models_[i].color, true);

		// ダメージカラーギミックの描画
		models_[i].gimmickDamageColor_->Draw(engine_, camera3d_, models_[i].modelHandle_, Vector3(1.0f, 1.0f, 1.0f));
	}
}


/// <summary>
/// 本体のワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 EnemyFairy::GetBodyWorldPosition() const
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
AABB EnemyFairy::GetCollisionAABB() const
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
void EnemyFairy::OnCollision(const BasePlayerBullet* playerBullet)
{
	// 基底クラスの衝突判定応答
	BaseEnemy::OnCollision(playerBullet);
}


/// <summary>
/// 弾を発射する
/// </summary>
void EnemyFairy::BulletShot()
{
	// 新規の弾の生成
	std::unique_ptr<EnemyBulletWeek> enemyBullet = std::make_unique<EnemyBulletWeek>();
	enemyBullet->SetGameTimer(gameTimer_);
	enemyBullet->Initialize(engine_, camera3d_, modelHandleStore_, worldTransform_->translation_);

	// ターゲットの方向に発射する
	enemyBullet->SetDirection(Normalize(target_->GetWorldTransform()->translation_ - worldTransform_->translation_));

	// ゲームシーンのリストに追加する
	gameScene_->EnemyBulletShot(std::move(enemyBullet));
}

/// <summary>
/// ステートを変更
/// </summary>
/// <param name="state"></param>
void EnemyFairy::ChangeState(State state)
{
	switch (state)
	{
	case kApproachingRear:
		// 後方から
		state_ = std::move(std::make_unique<EnemyFairyStateApproachingRear>(this));

		break;

	case kStop:
		// 停止
		state_ = std::move(std::make_unique<EnemyFairyStateStop>(this));

		break;

	case kAwayTop:
		// 上方向に離脱
		state_ = std::move(std::make_unique<EnemyFairyStateAwayTop>(this));

		break;
	}
}

/// <summary>
/// ダメージカラー
/// </summary>
void EnemyFairy::DamageColor()
{
	// ダメージカラーをリセットする
	for (uint32_t i = 0; i < kNumModel; i++)
	{
		models_[i].gimmickDamageColor_->Reset();
	}
}