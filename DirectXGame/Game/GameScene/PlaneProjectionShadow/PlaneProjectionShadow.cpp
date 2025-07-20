#include "PlaneProjectionShadow.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void PlaneProjectionShadow::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, uint32_t modelHandle, WorldTransform* worldTrasform)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(worldTrasform);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	modelHandle_ = modelHandle;
	casterWorldTransform_ = worldTrasform;


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// 平面投影行列
	shadowMatrix_.m[0][0] = 1.0f;
	shadowMatrix_.m[0][1] = 0.0f;
	shadowMatrix_.m[0][2] = 0.0f;
	shadowMatrix_.m[0][3] = 0.0f;

	shadowMatrix_.m[1][0] = 0.0f;
	shadowMatrix_.m[1][1] = 0.01f;
	shadowMatrix_.m[1][2] = 0.0f;
	shadowMatrix_.m[1][3] = 0.0f;

	shadowMatrix_.m[2][0] = 0.0f;
	shadowMatrix_.m[2][1] = 0.0f;
	shadowMatrix_.m[2][2] = 1.0f;
	shadowMatrix_.m[2][3] = 0.0f;

	shadowMatrix_.m[3][0] = 0.0f;
	shadowMatrix_.m[3][1] = 0.0f;
	shadowMatrix_.m[3][2] = 0.0f;
	shadowMatrix_.m[3][3] = 1.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void PlaneProjectionShadow::Update()
{
	// 投影元のワールドトランスフォームが設定されていたら
	if (casterWorldTransform_)
	{
		worldTransform_->worldMatrix_ = casterWorldTransform_->worldMatrix_ * shadowMatrix_;
	}

	// UVトランスフォームの更新
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void PlaneProjectionShadow::Draw()
{
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, Vector4(0.0f, 0.0f, 0.0f, 1.0f), false);
}