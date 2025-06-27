#include "Camera2D.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="width">横幅</param>
/// <param name="height">縦幅</param>
void Camera2D::Initialize(float width, float height)
{
	// 拡縮
	scale_ = { 1.0f , 1.0f , 1.0f };

	// 回転
	rotation_ = { 0.0f , 0.0f , 0.0f };

	// 移動
	translation_ = { 0.0f , 0.0f , 0.0f };

	// 画面の横幅
	screenWidth_ = width;

	// 画面の縦幅
	screenHeight_ = height;

	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix(scale_, rotation_, translation_));

	// 射影行列
	projectionMatrix_ = MakeOrthographicMatrix(-screenWidth_ / 2.0f, screenHeight_ / 2.0f, screenWidth_ / 2.0f, -screenHeight_ / 2.0f, 0.0f, 1.0f);
}

/// <summary>
/// 行列を更新する
/// </summary>
void Camera2D::UpdateMatrix()
{
	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix(scale_, rotation_, translation_));

	// 射影行列
	projectionMatrix_ = MakeOrthographicMatrix(-screenWidth_ / 2.0f, screenHeight_ / 2.0f, screenWidth_ / 2.0f, -screenHeight_ / 2.0f, 0.0f, 1.0f);
}

/// <summary>
/// ビュー行列を更新する
/// </summary>
void Camera2D::UpdateViewMatrix()
{
	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix(scale_, rotation_, translation_));
}

/// <summary>
/// 射影行列を更新する
/// </summary>
void Camera2D::UpdateProjectionMatrix()
{
	// 射影行列
	projectionMatrix_ = MakeOrthographicMatrix(-screenWidth_ / 2.0f, screenHeight_ / 2.0f, screenWidth_ / 2.0f, -screenHeight_ / 2.0f, 0.0f, 1.0f);
}