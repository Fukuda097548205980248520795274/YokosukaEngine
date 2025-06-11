#pragma once
#include "../YokosukaEngine/Include/YokosukaEngine.h"
#include "GameCamera/GameCamera.h"
#include "Axis/Axis.h"

class Game
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// デバッグカメラ有効化
	bool isDebugCameraActive_ = false;

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// 2Dカメラ
	std::unique_ptr<Camera2D> camera2d_ = nullptr;

	// ゲームカメラ
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	// デバッグのみで使用する
#ifdef _DEBUG

	// 軸方向表示
	std::unique_ptr<Axis> axis_ = nullptr;

	std::unique_ptr<WorldTransform> debugCameraWorldTransform_ = nullptr;
	std::unique_ptr<UvTransform> debugCameraUvTransform_ = nullptr;
	uint32_t debugCameraModelHandle_ = 0;

#endif


};