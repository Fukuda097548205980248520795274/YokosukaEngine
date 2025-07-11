#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

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


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// 終了フラグ
	bool isFinished_ = false;


	// 攻撃力
	int32_t power_ = 0;

	// 攻撃フラグ
	bool isAttack_ = false;
};

