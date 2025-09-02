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

	/// <summary>
	/// 向きのGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection()const { return direction_; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform* worldTransform_ = nullptr;


	// デッドゾーン
	float deadzone_ = 0.5f;

	// 速度
	float speed_ = 0.5f;

	// 向き
	Vector3 direction_ = { 1.0f , 0.0f , 0.0f };
};

