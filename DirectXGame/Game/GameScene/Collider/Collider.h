#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class Collider
{
public:

	/// <summary>
	/// 衝突半径のGetter
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// 衝突半径のSetter
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float radius) { radius_ = radius; }

	/// <summary>
	/// 衝突コールバック関数
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() const = 0;

	/// <summary>
	/// 衝突属性のGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }

	/// <summary>
	/// 衝突属性のSetter
	/// </summary>
	/// <param name="collisionAttribute"></param>
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/// <summary>
	/// 衝突マスクのGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionMask() { return collisionMask_; }

	/// <summary>
	/// 衝突マスクのSetter
	/// </summary>
	/// <param name="collisionMask"></param>
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private:

	// 衝突半径
	float radius_ = 2.0f;

	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xFFFFFFFF;

	// 衝突マスク（相手）
	uint32_t collisionMask_ = 0xFFFFFFFF;
};

