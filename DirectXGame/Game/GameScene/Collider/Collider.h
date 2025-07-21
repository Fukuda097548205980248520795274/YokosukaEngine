#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class Collider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Collider() = default;

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
	/// 衝突判定応答
	/// </summary>
	virtual void OnCollision() = 0;

	/// <summary>
	/// 中心座標のGetter
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetCenterPosition() const = 0;


private:

	// 衝突半径
	float radius_ = 1.5f;
};

