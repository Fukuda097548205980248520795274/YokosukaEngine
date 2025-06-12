#include "Vector1.h"

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
float Lerp(float start, float end, float t)
{
	return (1.0f - t) * start + t * end;
}