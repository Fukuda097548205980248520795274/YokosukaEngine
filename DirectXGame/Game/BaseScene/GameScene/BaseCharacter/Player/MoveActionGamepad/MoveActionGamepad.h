#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class MoveActionGamepad
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="worldTransform"></param>
	void Initialize(const YokosukaEngine* engine, WorldTransform* worldTransform);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform* worldTransform_ = nullptr;


	// デッドゾーン
	float deadzone_ = 0.5f;

	// 速度
	float speed_ = 0.5f;
};

