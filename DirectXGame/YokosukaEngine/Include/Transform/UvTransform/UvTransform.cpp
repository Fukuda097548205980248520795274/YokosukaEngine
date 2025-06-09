#include "UvTransform.h"

/// <summary>
/// 初期化
/// </summary>
void UvTransform::Initialize()
{
	// 拡縮
	scale_ = { 1.0f , 1.0f , 1.0f };

	// 回転
	rotation_ = { 0.0f , 0.0f , 0.0f };

	// 移動
	translation_ = { 0.0f , 0.0f , 0.0f };

	// ワールド行列
	worldMatrix_ = MakeAffineMatrix(scale_, rotation_, translation_);
}

/// <summary>
/// ワールド行列を更新する
/// </summary>
void UvTransform::UpdateWorldMatrix()
{
	// ワールド行列
	worldMatrix_ = MakeAffineMatrix(scale_, rotation_, translation_);
}