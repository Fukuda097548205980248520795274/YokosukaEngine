#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../BaseCharacter/Enemy/Enemy.h"

class LockOn
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera2d"></param>
	void Initialize(const YokosukaEngine* engine , const Camera3D* camera3d , const Camera2D* camera2d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ターゲットのGetter
	/// </summary>
	/// <returns></returns>
	const Enemy* GetTarget() const { return target_; }
	
	/// <summary>
	/// ターゲットの座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition()const;

	/// <summary>
	/// ロックオンしているかどうかのフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool ExistTaget()const { return target_ ? true : false; }


private:
	
	/// <summary>
	/// 最も近い敵を探索する
	/// </summary>
	/// <param name="enemies"></param>
	void Search(const std::list<std::unique_ptr<Enemy>>& enemies);

	/// <summary>
	/// ロックオンしている範囲
	/// </summary>
	bool LockOnRange();

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;


	// ロックオン対象
	const Enemy* target_ = nullptr;

	// 最小距離
	float minDistance_ = 10.0f;

	// 最大距離
	float maxDistance_ = 30.0f;

	// 角度範囲
	float angleRange_ = (std::numbers::pi_v<float> *2.0f) / 20.0f;
};

