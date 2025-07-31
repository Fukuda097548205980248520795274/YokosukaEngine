#include "BaseStageObject.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
/// <param name="rotation"></param>
void BaseStageObject::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,const float* gameFrame,
	float time, const Vector3& position, const Vector3& rotation)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(modelHandleStore);
	assert(gameFrame);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	modelHandleStore_ = modelHandleStore;
	gameFrame_ = gameFrame;
	time_ = time;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;
	worldTransform_->rotation_ = rotation;
}

/// <summary>
/// 更新処理
/// </summary>
void BaseStageObject::Update()
{
	// タイマーを進める
	timer_ += frameTimer_;
	
	// 時間を越えたら、終了する
	if (timer_ >= time_)
	{
		isFinished_ = true;
		return;
	}

	// ワールドトランスフォームの生成と初期化
	worldTransform_->UpdateWorldMatrix();
}