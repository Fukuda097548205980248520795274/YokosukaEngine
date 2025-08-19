#include "PlayerJet.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="modelHandleStore"></param>
void PlayerJet::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore)
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

	// モデルハンドルを取得する
	modelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kPlayer)[0];
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerJet::Update()
{
	// ワールドトランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerJet::Draw()
{
	engine_->DrawModel(worldTransform_.get(), camera3d_, modelHandle_, Vector4(0.0f, 0.0f, 0.0f, 1.0f), true);
}