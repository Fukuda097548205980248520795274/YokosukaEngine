#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../Enemy/Enemy.h"
#include "../Player/Player.h"

class LockOn
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine , const Camera3D* camera3d, const Camera2D* camera2d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Player* player,std::list<Enemy*>& enemies);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ターゲットのインスタンスのGetter
	/// </summary>
	/// <returns></returns>
	Enemy* GetTargetInstance() { return target_; }

	/// <summary>
	/// ターゲットフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsTarget() { return isTarget_; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;


	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_ = 0;

	// ロックオン対象
	Enemy* target_ = nullptr;

	// ターゲットの位置
	Vector2 targetPosisition_ = { 0.0f , 0.0f };

	// ターゲットしているかどうか
	bool isTarget_ = false;
};

