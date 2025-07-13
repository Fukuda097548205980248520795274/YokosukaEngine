#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class BasePlayerBullet;

class BaseEnemy
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

	/// <summary>
	/// 攻撃力のGetter
	/// </summary>
	/// <returns></returns>
	int32_t GetPower() const { return power_; }

	/// <summary>
	/// 攻撃フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsAttack() const { return isAttack_; }

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// 当たり判定用のAABBのGetter
	/// </summary>
	/// <returns></returns>
	AABB GetCollisionAABB(const Vector3& position) const;

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="playerBullet"></param>
	void OnCollision(const BasePlayerBullet* playerBullet);


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	// モデル構造体
	struct ModelStruct
	{
		// ワールドトランスフォーム
		std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

		// UVトランスフォーム
		std::unique_ptr<UvTransform> uvTransform_ = nullptr;

		// モデルハンドル
		uint32_t modelHandle_ = 0;

		// 色
		Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	};


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// 当たり判定の大きさ
	Vector3 hitSize_ = { 0.0f , 0.0f , 0.0f };

	// 体力
	int32_t hp_ = 0;

	// 終了フラグ
	bool isFinished_ = false;


	// 攻撃力
	int32_t power_ = 0;

	// 攻撃フラグ
	bool isAttack_ = false;


	// ダメージ音
	uint32_t soundHandleDamage_ = 0;

	// 撃破音
	uint32_t soundHandleDestroy_ = 0;
};

