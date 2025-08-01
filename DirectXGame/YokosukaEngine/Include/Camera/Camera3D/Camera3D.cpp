#include "Camera3D.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="width">横幅</param>
/// <param name="height">縦幅</param>
void Camera3D::Initialize(float width, float height)
{
	// 拡縮
	scale_ = { 1.0f , 1.0f , 1.0f };

	// 回転
	rotation_ = { 0.0f , 0.0f , 0.0f };

	// 移動
	translation_ = { 0.0f , 0.0f , 0.0f };

	// 画角
	fovY_ = 0.45f;

	// アスペクト比
	aspectRatio_ = width / height;

	// 近平面の距離
	nearClip_ = 0.1f;

	// 遠平面の距離
	farClip_ = 1000.0f;

	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix(scale_, rotation_, translation_));

	// 射影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
}

/// <summary>
/// 行列を更新する
/// </summary>
void Camera3D::UpdateMatrix()
{
	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix(scale_, rotation_, translation_));

	// 射影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
}

/// <summary>
/// ビュー行列を更新する
/// </summary>
void Camera3D::UpdateViewMatrix()
{
	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix(scale_, rotation_, translation_));
}

/// <summary>
/// 射影行列を更新する
/// </summary>
void Camera3D::UpdateProjectionMatrix()
{
	// 射影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
}

/// <summary>
/// 別のカメラの値で更新する
/// </summary>
/// <param name="camera3d"></param>
void Camera3D::UpdateOtherCamera(const Camera3D* camera3d)
{
	scale_ = camera3d->scale_;
	rotation_ = camera3d->rotation_;
	translation_ = camera3d->translation_;

	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix(scale_, rotation_, translation_));

	// 射影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
}