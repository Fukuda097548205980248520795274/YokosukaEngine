#include "StageObjectRainbow.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="modelHandleStore"></param>
/// <param name="gameFrame"></param>
/// <param name="time"></param>
/// <param name="position"></param>
/// <param name="rotation"></param>
void StageObjectRainbow::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const float* gameFrame,
	float time, const Vector3& position, const Vector3& rotation)
{
	// 基底クラスの初期化
	BaseStageObject::Initialize(engine, camera3d, modelHandleStore, gameFrame, time, position, rotation);

	// モデルを受け取る
	modelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kStageObjectRainbow)[0];
}

/// <summary>
/// 更新処理
/// </summary>
void StageObjectRainbow::Update()
{
	// 基底クラスの更新処理
	BaseStageObject::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void StageObjectRainbow::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), camera3d_, modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}