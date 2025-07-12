#pragma once
#include "../../Shape/AABB/AABB.h"
#include "../../Shape/Sphere/Sphere.h"
#include "../../Shape/Line/Line.h"
#include "../../Shape/Ray/Ray.h"
#include "../../Shape/Segment/Segment.h"

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="point">点</param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Vector3& point);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment);