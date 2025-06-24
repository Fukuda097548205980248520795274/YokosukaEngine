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

private:

	// 衝突半径
	float radius_ = 2.0f;
};

