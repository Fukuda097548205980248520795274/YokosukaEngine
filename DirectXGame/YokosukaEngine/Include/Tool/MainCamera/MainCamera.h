#pragma once
#include <memory>
#include "../../Camera/Camera3D/Camera3D.h"

class MainCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="screenWidth">画面の横幅</param>
	/// <param name="screenHeight">画面の縦幅</param>
	void Initialize(float screenWidth, float screenHeight);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ゲームカメラのインスタンスのGetter
	/// </summary>
	/// <returns></returns>
	Camera3D* GetGameCameraInstance() { return camera3d_.get(); }


private:

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;
};

