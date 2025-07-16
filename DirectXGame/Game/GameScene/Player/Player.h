#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class Player
{
public:

	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d);

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Draw
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	const WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

	/// <summary>
	/// メインカメラのSetter
	/// </summary>
	/// <param name="mainCamera"></param>
	void SetMainCamera(const MainCamera* mainCamera) { mainCamera_ = mainCamera; }


private:


	/// <summary>
	/// 入力操作
	/// </summary>
	void Input();

	/// <summary>
	/// 移動操作
	/// </summary>
	void Move();


	// engine
	const YokosukaEngine* engine_ = nullptr;

	// camera
	const Camera3D* camera3d_ = nullptr;

	// メインカメラ
	const MainCamera* mainCamera_ = nullptr;


	// worldTransform
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// uvTransform
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// modelHandle
	uint32_t modelHandle_ = 0;

	// 目標角度
	float toRotationY_ = 0.0f;
};

