#include "CollisionManager.h"

/// <summary>
/// クリア処理
/// </summary>
void CollisionManager::Clear()
{
	colliders_.clear();
}

/// <summary>
/// コライダーを登録する
/// </summary>
/// <param name="collider"></param>
void CollisionManager::PushCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

/// <summary>
/// 全ての衝突判定を行う
/// </summary>
void CollisionManager::CheckAllCollision()
{
	// リスト内ペアの総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		// コライダーA
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			// コライダーB
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

/// <summary>
/// コライダー2つの衝突判定と応答
/// </summary>
/// <param name="colliderA"></param>
/// <param name="colliderB"></param>
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
		return;

	// ワールド座標
	Sphere sphereA, sphereB;

	sphereA.center = colliderA->GetWorldPosition();
	sphereA.radius = colliderA->GetRadius();

	sphereB.center = colliderB->GetWorldPosition();
	sphereB.radius = colliderB->GetRadius();

	// 球同士の当たり判定
	if (IsCollision(sphereA, sphereB))
	{
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}