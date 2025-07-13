#include "BaseEnemy.h"
#include "../BasePlayerBullet/BasePlayerBullet.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void BaseEnemy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;


	// ダメージ音を読み込む
	soundHandleDamage_ = engine_->LoadSound("./Resources/Sounds/Se/enemy/damage/damage.mp3");

	// 撃破音を読み込む
	soundHandleDestroy_ = engine_->LoadSound("./Resources/Sounds/Se/enemy/damage/destroy.mp3");
}

/// <summary>
/// 更新処理
/// </summary>
void BaseEnemy::Update()
{
	// 体力がなくなったら消滅する
	if (hp_ <= 0)
	{
		// 撃破音
		engine_->PlaySoundData(soundHandleDestroy_ , 0.7f);

		isFinished_ = true;
	}

	// ワールドトランスフォームの更新処理
	worldTransform_->UpdateWorldMatrix();
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 BaseEnemy::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}

/// <summary>
/// 当たり判定用のAABBのGetter
/// </summary>
/// <returns></returns>
AABB BaseEnemy::GetCollisionAABB(const Vector3& position) const
{
	// AABB
	AABB aabb;

	aabb.max = hitSize_ + position;
	aabb.min = (-1.0f * hitSize_) + position;

	return aabb;
}

/// <summary>
/// 衝突判定応答
/// </summary>
/// <param name="playerBullet"></param>
void BaseEnemy::OnCollision(const BasePlayerBullet* playerBullet)
{
	// ダメージ音
	engine_->PlaySoundData(soundHandleDamage_ , 0.5f);

	// 弾の種類に合わせて体力が減る
	hp_ -= playerBullet->GetPower();
}