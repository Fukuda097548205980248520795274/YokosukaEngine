#pragma once
#define NOMINMAX
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class PoseBox
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera2d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera2D* camera2d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


	/// <summary>
	/// 開きのリセット
	/// </summary>
	void OpenReset(float time);

	/// <summary>
	/// 閉じのリセット
	/// </summary>
	void CloseReset(float time);


	// モード
	enum Mode
	{
		kOpen,
		kClose
	};

private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform2D> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// 白いテクスチャ
	uint32_t whiteTextureHandle_ = 0;



	// 現在のモード
	Mode mode_ = kOpen;

	// 最大サイズ
	Vector2 maxSize = Vector2(608.0f, 328.0f);

	// タイマー
	float timer_ = 0.0f;

	// 最大時間
	float maxTime_ = 0.0f;


	/// <summary>
	/// 開きの更新処理
	/// </summary>
	void OpenUpdate();

	/// <summary>
	/// 閉じの更新処理
	/// </summary>
	void CloseUpdate();
};

