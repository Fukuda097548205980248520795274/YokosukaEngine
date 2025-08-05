#include "BaseEnemyGroup.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void BaseEnemyGroup::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d,WorldTransform* worldTransform, const Vector3& position)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(worldTransform);
	
	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->SetParent(worldTransform);
	worldTransform_->translation_ = position;
}

/// <summary>
/// 更新処理
/// </summary>
void BaseEnemyGroup::Update()
{
	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
}