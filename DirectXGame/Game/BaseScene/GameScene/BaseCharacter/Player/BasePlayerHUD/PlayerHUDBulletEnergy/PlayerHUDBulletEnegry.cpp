#include "PlayerHUDBulletEnegry.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void PlayerHUDBulletEnegry::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore)
{
	// 基底クラスを初期化
	BasePlayerHUD::Initialize(engine, camera3d, modelHandleStore);

	// 位置
	worldTransform_->translation_ = Vector3(-5.7f, 3.1f, 19.3f);
	worldTransform_->rotation_ = Vector3(0.0f, -0.57f, 0.0f);

	// モデルを読み込む
	modelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kPlayerHUDBulletEnergy)[0];
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerHUDBulletEnegry::Update()
{
	// 割合を求める
	float t = *bulletEnergy_ / maxBulletEnergy_;

	// 割合で大きさを調整する
	worldTransform_->scale_.x = t;

	// 基底クラスを更新
	BasePlayerHUD::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerHUDBulletEnegry::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), camera3d_, modelHandle_, Vector4(0.0f, 1.0f, 0.0f, 0.2f), false);
}