#pragma once
#include <functional>

class TimedCall
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="f">コールバック関数</param>
	/// <param name="time">残り時間</param>
	TimedCall(std::function<void()> f, float time) : f_(f), time_(time) {};

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 完了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsComplate() { return isComplate_; }


private:

	// コールバック
	std::function<void()> f_;

	// 残り時間
	float time_ = 0.0f;

	// 完了フラグ
	bool isComplate_ = false;
};

