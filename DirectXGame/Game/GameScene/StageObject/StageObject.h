#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class StageObject
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d , const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() = 0;


protected:


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;
};

