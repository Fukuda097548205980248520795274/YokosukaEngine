#pragma once
#include "../../Math/Vector3/Vector3.h"

struct Plane
{
	// 始点
	Vector3 origin = Vector3(0.0f, 0.0f , 0.0f);

	// 法線
	Vector3 normal = Vector3(0.0f, 1.0f, 0.0f);

	// 距離
	float distance = 0.0f;
};