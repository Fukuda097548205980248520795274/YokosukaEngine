#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class Ground
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


private:

	// engine
	const YokosukaEngine* engine_ = nullptr;

	// camera
	const Camera3D* camera3d_ = nullptr;


	// worldTransform
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// uvTransform
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// modelHandle
	uint32_t modelHandle_ = 0;
};

