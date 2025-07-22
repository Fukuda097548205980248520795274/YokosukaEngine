#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../CollisionManager/CollisionTypeIdDef.h"

class Collider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Collider() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d);

	/// <summary>
	/// トランスフォームの更新処理
	/// </summary>
	void UpdateTransform();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

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
	/// 種別IDのGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetTypeID() const { return typeID_; }

	/// <summary>
	/// 種別IDのSetter
	/// </summary>
	/// <param name="typeID"></param>
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	virtual void OnCollision([[maybe_unused]]Collider* other) = 0;

	/// <summary>
	/// 中心座標のGetter
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetCenterPosition() const = 0;


protected:

	// エンジン
	const YokosukaEngine* engine_;

	// カメラ
	const Camera3D* camera3d_;
	

private:

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;


	// 衝突半径
	float radius_ = 1.5f;

	// 種別ID
	uint32_t typeID_ = 0u;
};

