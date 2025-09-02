#include "Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void Enemy::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore,
	const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラスを初期化する
	BaseCharacter::Initialize(engine, modelHandleStore, textureHandleStore, camera3d, position);

	// 本体のモデル
	modelBody_ = std::make_unique<Model>();
	modelBody_->Initialize(engine_, camera3d_, modelHandleStore_->GetModelHandle(ModelHandleStore::kSuzanne));
	modelBody_->worldTransform_->SetParent(worldTransform_.get());
	modelBody_->color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update()
{
	// 本体モデルの更新
	modelBody_->Update();

	// 基底クラスの更新
	BaseCharacter::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void Enemy::Draw()
{
	// 本体モデルの描画
	modelBody_->Draw();
}