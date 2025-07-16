#pragma once
#include <numbers>
#include <iostream>

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns>線形補間で求めた値</returns>
float Lerp(float start, float end, float t);

/// <summary>
/// 最短角度補間
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="t"></param>
/// <returns></returns>
float LerpShortAngle(float start, float end, float t);