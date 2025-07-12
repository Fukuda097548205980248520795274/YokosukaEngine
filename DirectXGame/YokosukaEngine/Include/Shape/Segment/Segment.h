#pragma once
#include "../../Math/Vector3/Vector3.h"

// 線分
struct Segment
{
	// 始点
	Vector3 origin;

	// 終点（差分ベクトル）
	Vector3 diff;
};