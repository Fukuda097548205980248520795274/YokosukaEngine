#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class EnemyBird;

class BaseEnemyBirdState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	BaseEnemyBirdState(EnemyBird* enemy) : enemy_(enemy) {};

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;


protected:

	// 敵
	EnemyBird* enemy_ = nullptr;

	// タイマー
	float timer_ = 0.0f;

	// 時間
	float time_ = 0.0f;
};

