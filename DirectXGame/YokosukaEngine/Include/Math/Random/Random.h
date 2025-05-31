#pragma once
#include <random>

class Random
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 乱数生成器のGetter
	/// </summary>
	/// <returns></returns>
	std::mt19937 GetRandomEngine() { return randomEngine_; }


private:

	/// <summary>
	/// 乱数生成器を生成する
	/// </summary>
	std::mt19937 CreateRandomEngine();

	// 乱数生成器
	std::mt19937 randomEngine_;
};

