#pragma once

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns>線形補間で求めた値</returns>
float Lerp(float start, float end, float t);