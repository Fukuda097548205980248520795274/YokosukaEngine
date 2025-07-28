#include "BaseBullet.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="modelHandleStore"></param>
void BaseBullet::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,
	const Vector3& position)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(modelHandleStore);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	modelHandleStore_ = modelHandleStore;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;
}

/// <summary>
/// 更新処理
/// </summary>
void BaseBullet::Update()
{
	// 直前の座標を記録する
	prevPosition_ = worldTransform_->translation_;

	// タイマーを進める
	shotTimer_ += 1.0f / 60.0f;

	// 発射時間を越えたら消滅する
	if (shotTimer_ >= shotTime_)
	{
		isFinished_ = true;
		return;
	}

	// 向きと速度で移動する
	worldTransform_->translation_ += direction_ * speed;

	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
}


/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 BaseBullet::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}