#pragma once
#include "../BaseBullet.h"

// 前方宣言
class BaseEnemy;

class BasePlayerBullet : public BaseBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,
		const Vector3& position) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override = 0;

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

	
};

