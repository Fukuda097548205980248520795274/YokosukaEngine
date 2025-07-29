#include "BaseEnemy.h"
#include "../../BaseBullet/BasePlayerBullet/BasePlayerBullet.h"
#include "../Player/Player.h"
#include "../../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void BaseEnemy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const Vector3& position, int32_t hp)
{
	// 基底クラスを初期化する
	BaseCharacter::Initialize(engine, camera3d, modelHandleStore,position, hp);


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
	// 体力がなくなったら撃破音を流す
	if (hp_ <= 0)
	{
		engine_->PlaySoundData(soundHandleDestroy_ , 0.7f);
	}

	// 基底クラスの更新
	BaseCharacter::Update();
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
/// 衝突判定応答
/// </summary>
/// <param name="playerBullet"></param>
void BaseEnemy::OnCollision(const BasePlayerBullet* playerBullet)
{
	// ダメージ音
	engine_->PlaySoundData(soundHandleDamage_ , 0.5f);

	// 弾の種類に合わせて体力が減る
	hp_ -= playerBullet->GetPower();

	// ダメージカラーをリセットする
	DamageColor();
}