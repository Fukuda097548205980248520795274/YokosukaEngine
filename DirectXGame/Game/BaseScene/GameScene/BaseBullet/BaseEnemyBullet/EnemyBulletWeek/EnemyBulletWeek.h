#pragma once
#include "../BaseEnemyBullet.h"

class EnemyBulletWeek : public BaseEnemyBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,
		const Vector3& position) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 本体のワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetBulletWorldPosition() const;


private:


	/*----------
	    本体
	----------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> bulletWorldTransform_ = nullptr;

	// モデルハンドル
	uint32_t bulletModelHandle_ = 0;

	// 弾のポイントライト
	std::unique_ptr<PointLight> bulletPointLight_ = nullptr;
};

