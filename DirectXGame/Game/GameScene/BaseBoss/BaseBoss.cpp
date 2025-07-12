#include "BaseBoss.h"

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void BaseBoss::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position)
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
}

/// <summary>
/// 更新処理
/// </summary>
void BaseBoss::Update()
{
	// ワールドトランスフォームの更新処理
	worldTransform_->UpdateWorldMatrix();
}