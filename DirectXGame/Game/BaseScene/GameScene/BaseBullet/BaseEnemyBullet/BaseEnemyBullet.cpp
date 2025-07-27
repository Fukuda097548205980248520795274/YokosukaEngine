#include "BaseEnemyBullet.h"


/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void BaseEnemyBullet::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, WorldTransform* parent)
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
	worldTransform_->SetParent(parent);
	worldTransform_->translation_ = position;
}

/// <summary>
/// 更新処理
/// </summary>
void BaseEnemyBullet::Update()
{
	// 直前の座標を記録する
	prevPosition_ = worldTransform_->translation_;

	// ワールドトランスフォームの更新処理
	worldTransform_->UpdateWorldMatrix();
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
Vector3 BaseEnemyBullet::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}


/// <summary>
/// 当たり判定用の線分のGetter
/// </summary>
/// <returns></returns>
Segment BaseEnemyBullet::GetCollisionSegment() const
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
void BaseEnemyBullet::OnCollision(const Player* player)
{
	// 終了する
	isFinished_ = true;
}