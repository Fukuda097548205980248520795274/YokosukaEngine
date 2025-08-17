#pragma once
#define NOMINMAX
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class PoseBg
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
	/// 色付けのリセット
	/// </summary>
	void ColoredReset(float time);

	/// <summary>
	/// 色消しのリセット
	/// </summary>
	void ColorFabesReset(float time);


	// モード
	enum Mode
	{
		kColored,
		kColorFabes
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
	Mode mode_ = kColored;

	// タイマー
	float timer_ = 0.0f;

	// 最大時間
	float maxTime_ = 0.0f;

	// 透明度
	float alpha_ = 0.0f;


	/// <summary>
	/// 色つけの更新処理
	/// </summary>
	void ColoredUpdate();

	/// <summary>
	/// 色消しの更新処理
	/// </summary>
	void ColorFabesUpdate();
};

