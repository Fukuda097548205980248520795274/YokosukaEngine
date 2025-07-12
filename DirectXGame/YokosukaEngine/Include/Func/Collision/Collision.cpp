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
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
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

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment)
{
	Vector3 tMin =
	{
		(aabb.min.x - segment.origin.x) / segment.diff.x,
		(aabb.min.y - segment.origin.y) / segment.diff.y,
		(aabb.min.z - segment.origin.z) / segment.diff.z
	};

	Vector3 tMax =
	{
		(aabb.max.x - segment.origin.x) / segment.diff.x,
		(aabb.max.y - segment.origin.y) / segment.diff.y,
		(aabb.max.z - segment.origin.z) / segment.diff.z
	};

	Vector3 tNear =
	{
		std::min(tMin.x , tMax.x),
		std::min(tMin.y , tMax.y),
		std::min(tMin.z , tMax.z)
	};

	Vector3 tFar =
	{
		std::max(tMin.x , tMax.x),
		std::max(tMin.y , tMax.y),
		std::max(tMin.z , tMax.z)
	};

	float tmin = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tmax = std::min(std::min(tFar.x, tFar.y), tFar.z);

	if (tmin <= 1.0f && tmax >= 0.0f)
	{
		if (tmin <= tmax)
		{
			return true;
		}
	}

	return false;
}