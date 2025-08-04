#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../ModelHandleStore/ModelHandleStore.h"
#include "../../../TextureHandleStore/TextureHandleStore.h"

class TitleHud
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="camera2d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d,
		const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// テクスチャハンドル格納場所
	const TextureHandleStore* textureHandleStore_ = nullptr;


	// スタート
	std::unique_ptr<WorldTransform2D> startWorldTransform_ = nullptr;
	std::unique_ptr<UvTransform> startUvTransform_ = nullptr;
	uint32_t startTextureHandle_ = 0;
};

