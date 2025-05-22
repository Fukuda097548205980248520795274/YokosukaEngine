#pragma once
#include "../../Math/Matrix4x4/Matrix4x4.h"

class Camera3D
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void Initialize(float width, float height);

	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// ビュー行列を更新する
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// 射影行列を更新する
	/// </summary>
	void UpdateProjectionMatrix();

	/// <summary>
	/// ビュープロジェクション行列のSetter
	/// </summary>
	/// <param name="viewMatrix"></param>
	/// <param name="projectionMatrix"></param>
	void SetViewProjectionMatrix(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) 
	{ viewMatrix_ = viewMatrix; projectionMatrix_ = projectionMatrix; }

	// 拡縮
	Vector3 scale_{};

	// 回転
	Vector3 rotation_{};

	// 移動
	Vector3 translation_{};

	// 画角
	float fovY_;

	// アスペクト比
	float aspectRatio_;

	// 近平面の距離
	float nearClip_;

	// 遠平面の距離
	float farClip_;

	// ビュー行列
	Matrix4x4 viewMatrix_{};

	// 射影行列
	Matrix4x4 projectionMatrix_{};
};

