#include "BaseScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void BaseScene::Initialize(const YokosukaEngine* engine , const ModelHandleStore* modelHandleStore)
{
	// nullptrチェック
	assert(engine);
	assert(modelHandleStore);

	// 引数を受け取る
	engine_ = engine;
	modelHandleStore_ = modelHandleStore;

	// シーンの生成と初期化
	scene_ = std::make_unique<Scene>();
	scene_->Initialize(engine);

	// メインカメラの取得
	mainCamera_ = scene_->GetMainCameraInstance();

	// 3Dカメラの取得
	camera3d_ = scene_->GetCamera3D();
}

/// <summary>
/// 更新処理
/// </summary>
void BaseScene::Update()
{
	// シーンの更新処理
	scene_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void BaseScene::Draw()
{
	// シーンの描画処理
	scene_->Draw();
}