#include "Random.h"

/// <summary>
/// 初期化
/// </summary>
void Random::Initialize()
{
	randomEngine_ = CreateRandomEngine();
}

/// <summary>
/// 乱数生成器を生成する
/// </summary>
std::mt19937 Random::CreateRandomEngine()
{
	std::random_device seedGenerater;
	std::mt19937 randomEngine(seedGenerater);
	return randomEngine;
}