#include "Vector1.h"

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns>線形補間で求めた値</returns>
float Lerp(float start, float end, float t)
{
	return (1.0f - t) * start + t * end;
}

/// <summary>
/// 最短角度補間
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="t"></param>
/// <returns></returns>
float LerpShortAngle(float start, float end, float t)
{
	// 角度の差
	float diff = end - start;

	diff = std::fmod(diff, std::numbers::pi_v<float> *2.0f);

	if (diff > std::numbers::pi_v<float>)
	{
		diff += -2.0f * std::numbers::pi_v<float>;
	} else if (diff < -std::numbers::pi_v<float>)
	{
		diff += 2.0f * std::numbers::pi_v<float>;
	}

	return start + diff * t;
}