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
/// <param name="sphere1">球</param>
/// <param name="sphere2">球</param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	if (std::pow(sphere1.radius + sphere2.radius, 2.0f) >=
		std::pow(sphere2.center.x - sphere1.center.x, 2.0f) + std::pow(sphere2.center.y - sphere1.center.y, 2.0f) + std::pow(sphere2.center.z - sphere1.center.z, 2.0f))
	{
		return true;
	}

	return false;
}