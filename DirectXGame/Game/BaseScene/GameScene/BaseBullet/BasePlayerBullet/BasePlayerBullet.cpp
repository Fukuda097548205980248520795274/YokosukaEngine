#include "BasePlayerBullet.h"
#include "../../BaseCharacter/BaseEnemy/BaseEnemy.h"


/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void BasePlayerBullet::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,
	const Vector3& position)
{
	// 基底クラスの初期化
	BaseBullet::Initialize(engine, camera3d, modelHandleStore, position);
}

/// <summary>
/// 更新処理
/// </summary>
void BasePlayerBullet::Update()
{
	// 基底クラスの更新処理
	BaseBullet::Update();
}

/// <summary>
/// 当たり判定用の線分のGetter
/// </summary>
/// <returns></returns>
Segment BasePlayerBullet::GetCollisionSegment() const
{
	// 線分
	Segment segment;

	segment.origin = worldTransform_->translation_;
	segment.diff = prevPosition_ - segment.origin;

	return segment;
}

/// <summary>
/// 衝突判定応答
/// </summary>
/// <param name="enemy"></param>
void BasePlayerBullet::OnCollision(const BaseEnemy* enemy)
{
	// 終了する
	isFinished_ = true;
}