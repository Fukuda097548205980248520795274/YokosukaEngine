#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class BaseEnemyGroup
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, WorldTransform* worldTransform, const Vector3& position);

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
	bool IsFinished()const { return isFinished_; }


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;


	// タイマー
	float timer_ = 0.0f;

	// 時間
	float time_ = 0.0f;


	// 終了フラグ
	bool isFinished_ = false;
};

