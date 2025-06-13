#include "Collision.h"

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="point">点</param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Vector3& point)
{
	if (point.x < aabb.max.x &&
		point.x > aabb.min.x)
	{
		if (point.y < aabb.max.y &&
			point.y > aabb.min.y)
		{
			if (point.z < aabb.max.z &&
				point.z > aabb.min.z)
			{
				return true;
			}
		}
	}

	return false;
}

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb1">AABB1</param>
/// <param name="aabb2">AABB2</param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if (aabb1.max.x >= aabb2.min.x &&
		aabb1.min.x <= aabb2.max.x)
	{
		if (aabb1.max.y >= aabb2.min.y &&
			aabb1.min.y <= aabb2.max.y)
		{
			if (aabb1.max.z >= aabb2.min.z &&
				aabb1.min.z <= aabb2.max.z)
			{
				return true;
			}
		}
	}

	return false;
}