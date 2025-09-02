#include "BaseCharacter.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void BaseCharacter::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore,
	const Camera3D* camera3d, const Vector3& position)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(modelHandleStore);
	assert(textureHandleStore);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	modelHandleStore_ = modelHandleStore;
	textureHandleStore_ = textureHandleStore;

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
	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 BaseCharacter::GetWorldPosition() const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}