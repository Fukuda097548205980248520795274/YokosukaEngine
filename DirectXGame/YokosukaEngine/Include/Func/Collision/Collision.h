#pragma once
#include "../../Shape/AABB/AABB.h"
#include "../../Math/Vector3/Vector3.h"

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="point">点</param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Vector3& point);