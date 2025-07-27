#include "BaseBullet.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="modelHandleStore"></param>
void BaseBullet::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,
	const Vector3& position, float shotTime)
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

	// 発射時間
	shotTime_ = shotTime;
}

/// <summary>
/// 更新処理
/// </summary>
void BaseBullet::Update()
{
	// タイマーを進める
	shotTimer_ += 1.0f / 60.0f;

	// 発射時間を越えたら消滅する
	if (shotTimer_ >= shotTime_)
	{
		isFinished_ = true;
		return;
	}

	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
}