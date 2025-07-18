#include "BasePlayerBullet.h"
#include "../BaseEnemy/BaseEnemy.h"


/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void BasePlayerBullet::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, WorldTransform* parent)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(parent);

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
void BasePlayerBullet::Update()
{
	// 直前の座標を記録する
	prevPosition_ = worldTransform_->translation_;

	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void BasePlayerBullet::Draw()
{

}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 BasePlayerBullet::GetWorldPosition() const
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