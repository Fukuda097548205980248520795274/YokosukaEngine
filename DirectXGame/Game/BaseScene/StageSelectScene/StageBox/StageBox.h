#pragma once
#define NOMINMAX
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

#include "../../../BaseSprite/BaseSprite.h"
#include "../../../TextureHandleStore/TextureHandleStore.h"

class StageBox
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera2d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera2D* camera2d , const TextureHandleStore* textureHandleStore);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(int32_t stage);


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

	// テクスチャハンドル格納場所
	const TextureHandleStore* textureHandleStore_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform2D> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// 白いテクスチャ
	uint32_t whiteTextureHandle_ = 0;



	// 現在のモード
	Mode mode_ = kOpen;

	// 最大サイズ
	Vector2 maxSize = Vector2(280.0f, 328.0f);

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


	// ステージ
	enum Stage
	{
		kTutorial,
		kStage1,
		kStage2,
		kStage3,
		kNumStage
	};

	std::unique_ptr<Sprite> spriteStage1_ = nullptr;
	std::unique_ptr<Sprite> spriteStage2_ = nullptr;
	std::unique_ptr<Sprite> spriteStage3_ = nullptr;
};

