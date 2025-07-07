#pragma once
#define NOMINMAX
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class Player
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d);

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

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	/*----------
	    中心
	----------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	/// <summary>
	/// 入力操作
	/// </summary>
	void Input();





	/*---------
	    本体
	---------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> bodyWorldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> bodyUvTransform_ = nullptr;

	// モデルハンドル
	uint32_t bodyModelHandle_ = 0;


	/// <summary>
	/// 行動 : 浮き : 初期化
	/// </summary>
	void BehaviorFloatInitialize();

	/// <summary>
	/// 行動 : 浮き : 更新処理
	/// </summary>
	void BehaviorFloatUpdate();

	// 浮きパラメータ
	float floatParameter_ = 0.0f;

	// 浮きパラメータ速度
	float kFloatPrameterVelocity = 0.05f;

	// 浮きパラメータ上限
	const float kMaxFloatParameter = std::numbers::pi_v<float> * 2.0f;

	// 浮き振れ幅
	float kFloatAmplitude = 0.3f;
};

