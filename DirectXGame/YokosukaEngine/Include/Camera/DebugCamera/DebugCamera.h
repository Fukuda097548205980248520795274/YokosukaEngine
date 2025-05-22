#pragma once
#include "../../Math/Matrix4x4/Matrix4x4.h"

class DebugCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(float screenWidth , float screenHeight);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0.0f , 0.0f , 0.0f };

	// ローカル座標
	Vector3 translation_{ 0.0f , 0.0f , -50.0f };

	// ビュー行列
	Matrix4x4 viewMatrix_ = {};

	// 射影行列
	Matrix4x4 projectionMatrix_ = {};


private:

	// 画面の横幅
	float screenWidth_ = 0.0f;

	// 画面の縦幅
	float screenHeight_ = 0.0f;
};

