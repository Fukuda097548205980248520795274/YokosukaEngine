#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../ModelHandleStore/ModelHandleStore.h"
#include "../../../TextureHandleStore/TextureHandleStore.h"

class PlayerStateHud
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



	// Aボタン
	std::unique_ptr<WorldTransform2D> aButtonWorldTransform_ = nullptr;
	std::unique_ptr<UvTransform> aButtonUvTransform_ = nullptr;
	uint32_t aButtonTextureHandle_ = 0;

	// Lスティック
	std::unique_ptr<WorldTransform2D> lStickWorldTransform_ = nullptr;
	std::unique_ptr<UvTransform> lStickUvTransform_ = nullptr;
	uint32_t lStickTextureHandle_ = 0;

	// Lスティック押し込み
	std::unique_ptr<WorldTransform2D> lStickThumbWorldTransform_ = nullptr;
	std::unique_ptr<UvTransform> lStickThumbUvTransform_ = nullptr;
	uint32_t lStickThumbTextureHandle_ = 0;
};

