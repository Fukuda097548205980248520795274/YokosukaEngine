#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class EnemyJet;

class BaseEnemyJetState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	BaseEnemyJetState(EnemyJet* enemy) : enemy_(enemy) {};

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;


protected:

	// 敵
	EnemyJet* enemy_ = nullptr;

	// タイマー
	float timer_ = 0.0f;

	// 時間
	float time_ = 0.0f;
};

