#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../../ModelHandleStore/ModelHandleStore.h"

class BaseStageObject
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	/// <param name="rotation"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,const float* gameFrame,
		float time ,const Vector3& position, const Vector3& rotation);

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

	/// <summary>
	/// タイマーを無限にする
	/// </summary>
	void InfiniteTimer() { frameTimer_ = 0.0f; }



protected:


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// ゲームフレーム
	const float* gameFrame_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// 終了フラグ
	bool isFinished_ = false;


	// タイマー
	float timer_ = 0.0f;

	// 1フレームの秒数
	float frameTimer_ = 1.0f / 60.0f;

	// 時間
	float time_ = 0.0f;
};

