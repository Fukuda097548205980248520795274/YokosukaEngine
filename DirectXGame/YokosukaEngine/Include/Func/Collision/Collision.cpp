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