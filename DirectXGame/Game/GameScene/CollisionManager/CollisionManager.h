#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../Collider/Collider.h"
#include "../GlobalVariables/GlobalVariables.h"

class CollisionManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize();
	
	/// <summary>
	/// トランスフォームの更新処理
	/// </summary>
	void UpdateTransform();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

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

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVaribles();

	// 可視コライダー
	bool enableVisibleCollider_ = false;

	// コライダーのリスト
	std::list<Collider*> colliders_;
};

