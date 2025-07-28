#include "BaseCharacter.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="modelHandleStore"></param>
void BaseCharacter::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const Vector3& position, int32_t hp)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(modelHandleStore);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	modelHandleStore_ = modelHandleStore;
	hp_ = hp;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;
}

/// <summary>
/// 更新処理
/// </summary>
void BaseCharacter::Update()
{
	// 体力がなくなったら消滅する
	if (hp_ <= 0)
	{
		isFinished_ = true;
		return;
	}

	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
}