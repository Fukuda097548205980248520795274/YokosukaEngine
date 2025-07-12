#pragma once
#include "../../Math/Vector3/Vector3.h"

// 半直線
struct Ray
{
	// 始点
	Vector3 origin;

	// 終点（差分ベクトル）
	Vector3 diff;
};