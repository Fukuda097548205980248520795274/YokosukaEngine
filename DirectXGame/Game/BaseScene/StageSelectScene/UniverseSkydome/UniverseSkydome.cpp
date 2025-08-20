#include "UniverseSkydome.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="modelHandleStore"></param>
void UniverseSkydome::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore)
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

	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();
	uvTransform_->scale_ = Vector3(17.0f, 15.0f, 15.0f);

	// モデルハンドルを取得する
	modelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kUniverse)[0];
}

/// <summary>
/// 更新処理
/// </summary>
void UniverseSkydome::Update()
{
	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void UniverseSkydome::Draw()
{
	engine_->DrawModel(worldTransform_.get(),uvTransform_.get(), camera3d_, modelHandle_, Vector4(0.5f, 0.5f, 0.5f, 1.0f), false);
}