#include "WorldTransform2D.h"

/// <summary>
/// 初期化
/// </summary>
void WorldTransform2D::Initialize(const Vector2& anchor)
{
	// 拡縮
	scale_ = { 1.0f , 1.0f , 1.0f };

	// 回転
	rotation_ = { 0.0f,0.0f,0.0f };

	// 移動
	translation_ = { 0.0f,0.0f,0.0f };

	// アンカーポイント
	anchor_ = anchor;

	// ワールド行列
	worldMatrix_ = MakeAffineMatrix(scale_, rotation_, translation_);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="scale"></param>
/// <param name="rotation"></param>
/// <param name="translation"></param>
void WorldTransform2D::Initialize(const Vector2& anchor,const Vector3& scale, const Vector3& rotation, const Vector3& translation)
{
	// 拡縮
	scale_ = scale;

	// 回転
	rotation_ = rotation;

	// 移動
	translation_ = translation;

	// アンカーポイント
	anchor_ = anchor;

	// ワールド行列
	worldMatrix_ = MakeAffineMatrix(scale_, rotation_, translation_);
}

/// <summary>
/// ワールド行列を更新する
/// </summary>
void WorldTransform2D::UpdateWorldMatrix()
{
	// ワールド行列
	worldMatrix_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 親のワールド行列で座標変換する
	if (parent_)
	{
		worldMatrix_ = worldMatrix_ * MakeAffineMatrix(parent_->scale_, parent_->rotation_, parent_->translation_);
	}
}