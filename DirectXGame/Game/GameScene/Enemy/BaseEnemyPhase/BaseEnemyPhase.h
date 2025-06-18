#pragma once
#include <string>

// 前方宣言
class Enemy;

class BaseEnemyPhase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">状態名</param>
	/// <param name="enemy">敵のポインタ</param>
	BaseEnemyPhase(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy) {};

	// 毎フレームの処理
	virtual void Update() = 0;

	// デバッグログ出力
	//virtual void DebugLog();


protected:

	// 状態名
	std::string name_;

	// 操作対象の敵
	Enemy* enemy_ = nullptr;
};

