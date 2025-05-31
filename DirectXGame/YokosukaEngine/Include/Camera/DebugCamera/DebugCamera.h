#pragma once
#include "../../Math/Matrix4x4/Matrix4x4.h"

// 前方宣言
class YokosukaEngine;

class DebugCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


	// エンジン
	YokosukaEngine* engine_ = nullptr;;

	// ローカル拡縮
	Vector3 scale_ = { 1.0f , 1.0f , 1.0f };

	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0.0f , 0.0f , 0.0f };

	// ローカル座標
	Vector3 translation_ = { 0.0f , 0.0f , 0.0f };
};

