#include "DebugCamera.h"

/// <summary>
/// 初期化
/// </summary>
void DebugCamera::Initialize(float screenWidth, float screenHeight)
{
	// 引数を受け取る
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;


	// ビュー行列
	viewMatrix_ = MakeInverseMatrix(MakeAffineMatrix({ 1.0f , 1.0f , 1.0f }, rotation_, translation_));

	// 射影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, screenWidth_ / screenHeight_, 0.1f, 1000.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void DebugCamera::Update()
{
	
}