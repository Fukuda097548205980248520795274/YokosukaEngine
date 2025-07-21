#pragma once
#include "../Collider/Collider.h"

class CollisionManager
{
public:

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// コライダーを追加する
	/// </summary>
	/// <param name="collider"></param>
	void AddCollider(Collider* collider);

	/// <summary>
	/// コライダー2つの衝突判定
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 全ての当たり判定をチェックする
	/// </summary>
	void CheckAllCollisions();


private:

	std::list<Collider*> colliders_;
};

