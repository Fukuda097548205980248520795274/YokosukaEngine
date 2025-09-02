#include "PlayerVisibility.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="worldTransform"></param>
void PlayerVisibility::Initialize(WorldTransform* worldTransform)
{
	// nullptrチェック
	assert(worldTransform);

	// 引数を受け取る
	worldTransform_ = worldTransform;

	// 上側のワールドトランスフォームの生成と初期化
	topWorldTransform_ = std::make_unique<WorldTransform>();
	topWorldTransform_->Initialize();
	topWorldTransform_->SetParent(worldTransform_);
	topWorldTransform_->rotation_.x = (-std::numbers::pi_v<float> / 2.0f) - 0.2f;

	// 下側のワールドトランスフォームの生成と初期化
	bottomWorldTransform_ = std::make_unique<WorldTransform>();
	bottomWorldTransform_->Initialize();
	bottomWorldTransform_->SetParent(worldTransform_);
	bottomWorldTransform_->rotation_.x = (std::numbers::pi_v<float> / 2.0f) + 0.2f;


	planeBack_.origin = Vector3(0.0f, 0.0f, 0.0f);
	
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerVisibility::Update()
{
	// 後ろ側の平面の法線を取得する
	Vector3 planeBackNormal = Normalize(TransformNormal(Vector3(0.0f, 0.0f, -1.0f), worldTransform_->worldMatrix_));
	planeBack_.normal = planeBackNormal;
	planeBack_.origin = Vector3(worldTransform_->worldMatrix_.m[3][0], worldTransform_->worldMatrix_.m[3][1], worldTransform_->worldMatrix_.m[3][2]);

	// ワールドトランスフォームの更新
	topWorldTransform_->UpdateWorldMatrix();
	bottomWorldTransform_->UpdateWorldMatrix();

	// 上下の平面の法線を取得する
	planeTop_.normal = Normalize(TransformNormal(Vector3(0.0f, 0.0f, 1.0f), topWorldTransform_->worldMatrix_));
	planeTop_.origin = 
		Vector3(topWorldTransform_->worldMatrix_.m[3][0], topWorldTransform_->worldMatrix_.m[3][1], topWorldTransform_->worldMatrix_.m[3][2]);

	planeBottom_.normal = Normalize(TransformNormal(Vector3(0.0f, 0.0f, 1.0f), bottomWorldTransform_->worldMatrix_));
	planeBottom_.origin = 
		Vector3(bottomWorldTransform_->worldMatrix_.m[3][0], bottomWorldTransform_->worldMatrix_.m[3][1], bottomWorldTransform_->worldMatrix_.m[3][2]);
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerVisibility::Draw()
{

}