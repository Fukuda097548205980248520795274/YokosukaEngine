#pragma once
#include "../../Math/Vector3/Vector3.h"
#include "../../Shape/AABB/AABB.h"

// 場
struct Feild
{
	// 加速度
	Vector3 acceleration;

	// AABB
	AABB area;
};