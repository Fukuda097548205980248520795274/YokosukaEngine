#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class EnemyDevil;

class BaseEnemyDevilBehavior
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	BaseEnemyDevilBehavior(EnemyDevil* enemy) : enemy_(enemy) {}

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished()const { return isFinished_; }


protected:

	// 敵
	EnemyDevil* enemy_ = nullptr;

	// 終了フラグ
	bool isFinished_ = false;
};

