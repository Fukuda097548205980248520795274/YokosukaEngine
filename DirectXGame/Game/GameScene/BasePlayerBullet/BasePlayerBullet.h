#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class BaseEnemy;

class BasePlayerBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position, WorldTransform* parent);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

	/// <summary>
	/// 攻撃力のGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetPower() const { return power_; }

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// 当たり判定用の線分のGetter
	/// </summary>
	/// <returns></returns>
	Segment GetCollisionSegment() const;

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="enemy"></param>
	void OnCollision(const BaseEnemy* enemy);


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;



	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// 1フレーム前の座標
	Vector3 prevPosition_ = { 0.0f , 0.0f , 0.0f };

	// 終了フラグ
	bool isFinished_ = false;

	// 攻撃力
	uint32_t power_ = 0;
};

