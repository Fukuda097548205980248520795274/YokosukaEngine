#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../MainCamera/MainCamera.h"
#include "../Axis/Axis.h"

class TitleScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
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

	// ゲームカメラ
	std::unique_ptr<MainCamera> mainCamera_ = nullptr;

	// デバッグのみで使用する
#ifdef _DEBUG

	// 軸方向表示
	std::unique_ptr<Axis> axis_ = nullptr;

#endif


};

