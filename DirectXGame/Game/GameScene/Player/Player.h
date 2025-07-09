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

	/// <summary>
	/// 移動操作
	/// </summary>
	void Move();

	/// <summary>
	/// ゲームパッドでの移動操作
	/// </summary>
	void MoveGamepad();

	/// <summary>
	/// キーボードでの移動操作
	/// </summary>
	void MoveKeybord();


	// 移動速度
	const float speed = 0.4f;





	/*---------
	    本体
	---------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> bodyWorldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> bodyUvTransform_ = nullptr;

	// モデルハンドル
	uint32_t bodyModelHandle_ = 0;



	/*-------------
	    ギミック
	-------------*/

	/*   浮き   */

	/// <summary>
	/// ギミック : 浮き : 初期化
	/// </summary>
	void GimmickFloatInitialize();

	/// <summary>
	/// ギミック : 浮き : 更新処理
	/// </summary>
	void GimmickFloatUpdate();

	// 浮きパラメータ
	float floatParameter_ = 0.0f;

	// 浮きパラメータ速度
	float kFloatPrameterVelocity = 0.05f;

	// 浮きパラメータ上限
	const float kMaxFloatParameter = std::numbers::pi_v<float> * 2.0f;

	// 浮き振れ幅
	float kFloatAmplitude = 0.3f;


	/*------------------
	    移動ビヘイビア
	------------------*/

	/// <summary>
	/// 移動ビヘイビアの更新処理
	/// </summary>
	void UpdateMoveBehavior();

	enum MoveBehavior
	{
		kStraight,
		kLeft,
		kRight,
		kNumMoveBehavior
	};

	// 現在の移動ビヘイビア
	MoveBehavior moveBehavior_ = kStraight;

	// 目標角度
	const float kMoveBehaviorGoalRadian[kNumMoveBehavior] = { 0.0f , std::numbers::pi_v<float> / 4.0f , -std::numbers::pi_v<float> / 4.0f };
};

