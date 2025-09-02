#pragma once
#define NOMINMAX
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include <algorithm>

// 前方宣言
class Enemy;

class ScaleActionGamepad
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
	/// 操作フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsOperation()const { return isOperation_; }

	/// <summary>
	/// ターゲットリストのSetter
	/// </summary>
	/// <param name="target"></param>
	void SetTargets(std::list<Enemy*> target) { targets_ = target; }

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

	// ターゲットリスト
	std::list<Enemy*> targets_;

	// 向き
	Vector3 direction_ = { 1.0f , 0.0f , 0.0f };


	// デッドゾーン
	float deadzone_ = 0.5f;

	// 速度
	float speed_ = 0.1f;

	// 操作中かどうか
	bool isOperation_ = false;


	// 最大サイズ
	const float kMaxSize = 3.0f;

	// 最小サイズ
	const float kMinSize = 0.3f;
};

