#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../Collider/Collider.h"

class CollisionManager
{
public:

	/// <summary>
	/// クリア処理
	/// </summary>
	void Clear();

	/// <summary>
	/// コライダーを登録する
	/// </summary>
	/// <param name="collider"></param>
	void PushCollider(Collider* collider);

	/// <summary>
	/// 全ての衝突判定を行う
	/// </summary>
	void CheckAllCollision();


private:

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	// コライダーのリスト
	std::list<Collider*> colliders_;
};

