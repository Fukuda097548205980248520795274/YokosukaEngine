#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../ModelHandleStore/ModelHandleStore.h"

class BaseBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="modelHandleStore"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,
		const Vector3& position);

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
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;

	/// <summary>
	/// 攻撃力のGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetPower()const { return power_; }

	/// <summary>
	/// ゲームタイマーのSetter
	/// </summary>
	/// <param name="gameTimer"></param>
	/// <returns></returns>
	void SetGameTimer(const float* gameTimer) { gameTimer_ = gameTimer; }

	/// <summary>
	/// トランスフォームの親のSetter
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetParent(WorldTransform* worldTransform) { worldTransform_->SetParent(worldTransform); }

	/// <summary>
	/// 向きのSetter
	/// </summary>
	/// <param name="direction"></param>
	void SetDirection(Vector3 direction) { direction_ = Normalize(direction); }


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;


	// 向き
	Vector3 direction_ = { 0.0f , 0.0f , 0.0f };

	// 移動速度
	float speed = 0.0f;


	// 終了フラグ
	bool isFinished_ = false;

	// 発射タイマー
	float shotTimer_ = 0.0f;

	// 発射時間
	float shotTime_ = 0.0f;



	// 1フレーム前の座標
	Vector3 prevPosition_ = { 0.0f , 0.0f , 0.0f };

	// 攻撃力
	uint32_t power_ = 0;



	// ゲームタイマー
	const float* gameTimer_ = nullptr;
};

