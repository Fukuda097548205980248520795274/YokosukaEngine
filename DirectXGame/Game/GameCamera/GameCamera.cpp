#include "GameCamera.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="screenWidth">画面の横幅</param>
/// <param name="screenHeight">画面の縦幅</param>
void GameCamera::Initialize(float screenWidth, float screenHeight)
{
	// カメラの生成と初期化
	camera3d_ = std::make_unique<Camera3D>();
	camera3d_->Initialize(screenWidth, screenHeight);
}

/// <summary>
/// 更新処理
/// </summary>
void GameCamera::Update()
{
	// 3Dカメラ更新
	camera3d_->UpdateMatrix();
}