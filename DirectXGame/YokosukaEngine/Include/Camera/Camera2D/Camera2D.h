#pragma once
#include "../../Math/Matrix4x4/Matrix4x4.h"

class Camera2D
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

	// 拡縮
	Vector3 scale_{};

	// 回転
	Vector3 rotation_{};

	// 移動
	Vector3 translation_{};

	// ビュー行列
	Matrix4x4 viewMatrix_{};

	// 射影行列
	Matrix4x4 projectionMatrix_{};

	// 画面の横幅
	float screenWidth_ = 0.0f;

	// 画面の縦幅
	float screenHeight_ = 0.0f;
};

