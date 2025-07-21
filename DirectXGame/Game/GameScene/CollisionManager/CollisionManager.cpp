#include "CollisionManager.h"

/// <summary>
/// リセット
/// </summary>
void CollisionManager::Reset()
{
	// リストを空っぽにする
	colliders_.clear();
}

/// <summary>
/// コライダーを追加する
/// </summary>
/// <param name="collider"></param>
void CollisionManager::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

/// <summary>
/// コライダー2つの衝突判定
/// </summary>
/// <param name="colliderA"></param>
/// <param name="colliderB"></param>
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	// nullptrチェック
	assert(colliderA);
	assert(colliderB);

	Vector3 positionA = colliderA->GetCenterPosition();
	Vector3 positionB = colliderB->GetCenterPosition();

	// 差分ベクトル
	float subtract = Length(positionA - positionB);

	if (colliderA->GetRadius() + colliderB->GetRadius() >= subtract)
	{
		// コライダーAの衝突判定
		colliderA->OnCollision();

		// コライダーBの衝突判定
		colliderB->OnCollision();
	}
}

/// <summary>
/// 全ての当たり判定をチェックする
/// </summary>
void CollisionManager::CheckAllCollisions()
{
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		++itrB;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}