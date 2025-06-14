#pragma once
#define NOMINMAX
#include <memory>
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class Player;

class MainCamera
{
public:

	// 矩形
	struct Rect
	{
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

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

	/// <summary>
	/// 追従対象のSetter
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Player* target) { target_ = target; }

	/// <summary>
	/// リセット関数
	/// </summary>
	void Reset();

	/// <summary>
	/// カメラ移動範囲のSetter
	/// </summary>
	/// <param name="area"></param>
	void SetMovableArea(Rect area) { movableArea_ = area; }


private:

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// 追従対象
	const Player* target_ = nullptr;

	// 追従対象とカメラの座標の差
	Vector3 targetOffset_ = { 0.0f , 5.0f,  -45.0f };

	// カメラ移動範囲
	Rect movableArea_ = { 0.0f , 100.0f , 0.0f , 100.0f };


	// 座標補間割合
	const float kInterpolationRate = 0.1f;

	// 速度掛け率
	const float kVelocityBias = 20.0f;

	// 目標座標
	Vector3 goalPosition_ = { 0.0f , 0.0f , 0.0f };

	// 追従対象へのカメラ移動範囲
	Rect targetMargin_ = { -6.0f , 6.0f , -0.1f , 0.1f };
};

